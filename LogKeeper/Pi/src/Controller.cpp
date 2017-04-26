/*
Copyright_License {

  Glider Operations.
  Copyright (C) 2013-2017 Peter F Bradshaw
  A detailed list of copyright holders can be found in the file "AUTHORS".

  This program is free software; you can redistribute it and/or
  modify it under the terms of the GNU General Public License
  as published by the Free Software Foundation; either version 2
  of the License, or (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program; if not, write to the Free Software
  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
}
*/

#include "Controller.hpp"
#include <boost/asio.hpp>
#include <boost/asio/error.hpp>
#include <boost/bind.hpp>
#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <errno.h>

#ifdef PI
#include <wiringPi.h>
#include <wiringPiI2C.h>
#else
#define LOW 0
#define HIGH 1
#endif

ProgramState   program_state   = ProgramState::DOWN;
ProcessorState processor_state = ProcessorState::UP;
int i2c = -1;

void arduinoPoll(const boost::system::error_code &,
                 boost::asio::deadline_timer *);
void initIo();
void executeStatus(const boost::system::error_code &e,
                   boost::asio::deadline_timer *);
void shutdownProgram();
void runupProgram();
void shutdownProcessor();
void checkIfProgramDown();
int readPin(int pin);

//------------------------------------------------------------------------------
void
arduinoPoll(const boost::system::error_code &e,
            boost::asio::deadline_timer *t)
  {
  struct tm result;
  time_t tl;
#ifdef PI
  if (e == boost::asio::error::operation_aborted)
    return;

#else // PI
  if (e == boost::asio::error::operation_aborted)
    {
    std::cout << "Poll() : abort" << std::endl;
    return;
    }
#endif  // PI
   
   tl = time(nullptr);
   localtime_r(&tl, &result);
   if (result.tm_wday == 0 || result.tm_wday == 5 || result.tm_wday == 6)
     {  // Friday, Saturday or Sunday.
#if PI
     wiringPiI2CWrite (i2c, 1);
#else // PI
     std::cout << "Flarm enable" << std::endl;
#endif  // PI
     }
   else
     {
#ifdef PI
     wiringPiI2CWrite(i2c, 0);
#else // PI
     std::cout << "Flarm disable" << std::endl;
#endif  // PI
     }

  t->expires_at(t->expires_at() + boost::posix_time::seconds(5));
  t->async_wait(boost::bind(arduinoPoll,
                            boost::asio::placeholders::error,
                            t)
               );
  }

//------------------------------------------------------------------------------
void
initIo()
  {
#ifdef PI
  wiringPiSetup();
  if ((::i2c = wiringPiI2CSetup(1)) == -1)
    {
    char buf[128];

    std::cerr << strerror_r(errno, buf, sizeof(buf) / sizeof(char))
              << std::endl;
    exit(1);
    }

  pinMode(0, INPUT);
  pinMode(1, INPUT);
  pullUpDnControl(0, PUD_UP);
  pullUpDnControl(1, PUD_UP);
#else
  std::cout << "initIo()" << std::endl;
#endif  // PI
  }

//------------------------------------------------------------------------------
void
executeStatus(const boost::system::error_code &e,
              boost::asio::deadline_timer *t)
  {
  if (readPin(0) == HIGH)
    {
    switch (program_state)
      {
      case ProgramState::UP:
        shutdownProgram();
        break;
      case ProgramState::SHUTTINGDOWN:
      default:
        break;
      case ProgramState::DOWN:
        {
        switch (processor_state)
          {
          case ProcessorState::UP:
            shutdownProcessor();
            break;
          default:
            break;
          }
        }
      }
    }
  if (readPin(1) == HIGH)
    {
    switch (program_state)
      {
      case ProgramState::UP:
        shutdownProgram();
        break;
      case ProgramState::SHUTTINGDOWN:
      case ProgramState::DOWN:
      default:
        break;
      }
    }
  else
    {
    switch (program_state)
      {
      case ProgramState::UP:
      case ProgramState::SHUTTINGDOWN:  // If SHUTTINGDOWN let it proceed to DOWN.
        break;
      case ProgramState::DOWN:
        runupProgram();
        break;
      }
    }
  if (program_state == ProgramState::SHUTTINGDOWN)
    checkIfProgramDown();

  t->expires_at(t->expires_at() + boost::posix_time::seconds(1));
  t->async_wait(boost::bind(arduinoPoll,
                            boost::asio::placeholders::error,
                            t)
               );

  }

//------------------------------------------------------------------------------
#ifdef PI
void
shutdownProgram()
  {
  system("/etc/init.d/program stop");
  ::program_state = ProgramState::SHUTTINGDOWN;
  }
#else
void
shutdownProgram()
  {
  std::cout << "/etc/init.d/program stop" << std::endl;
  ::program_state = ProgramState::SHUTTINGDOWN;
  }
#endif  // PI

//------------------------------------------------------------------------------
#ifdef PI
void
runupProgram()
  {
  system("/etc/init.d/program start");
  ::program_state = ProgramState::UP;
  }
#else // PI
void
runupProgram()
  {
  std::cout << "/etc/init.d/program start" << std::endl;
  ::program_state = ProgramState::UP;
  }
#endif  // PI

//------------------------------------------------------------------------------
#ifdef PI
void
shutdownProcessor()
  {
  system("/sbin/poweroff");
  ::processor_state = ProcessorState::SHUTTINGDOWN;
  }
#else // PI
void
shutdownProcessor()
  {
  std::cout << "/sbin/poweroff" << std::endl;
  ::processor_state = ProcessorState::SHUTTINGDOWN;
  }
#endif  // PI

//------------------------------------------------------------------------------
#ifdef PI
void
checkIfProgramDown()
  {
  if (system("/etc/init.d/program status") == 3)
    ::program_state = ProgramState::DOWN;
  }
#else // PI
void
checkIfProgramDown()
  {
  int is;
  std::cout << "/etc/init.d/program status ? : ";
  std::cin >> is;
  if (is == 3)
    ::program_state = ProgramState::DOWN;
  }
#endif  // PI

//------------------------------------------------------------------------------
#ifdef PI
int
readPin(int pin)
  {
  return digitalRead(pin);
  }
#else // PI
int
readPin(int pin)
  {
  int is;
  std::cout << "GPIO " << pin << " ? : ";
  std::cin >> is;
  return (is == 0) ? LOW : HIGH;
  }
#endif  // PI

//------------------------------------------------------------------------------
int
main(int argc, const char *argv[])
  {
  initIo();
  boost::asio::io_service actor;

  boost::asio::deadline_timer i2ctimer(actor, boost::posix_time::seconds(2));
  i2ctimer.async_wait(boost::bind(arduinoPoll,
                                  boost::asio::placeholders::error,
                                  &i2ctimer)
                     );
  boost::asio::deadline_timer polltimer(actor, boost::posix_time::seconds(1));
  polltimer.async_wait(boost::bind(executeStatus,
                                   boost::asio::placeholders::error,
                                   &polltimer)
                      );

  actor.run();

  return 0;
  }

//------------------------------------------------------------------------------
/*
 * This is necessary for the boot lib.
 */
void
boost::throw_exception(std::exception const &)
  {
  }
