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

#include "Controller.common.test.hpp"
#include "Arduino.h"
#include "I2Cdev.h"

#include <Wire.h>

#include "ControllerStates.hpp"

#define PIN00  0
#define PIN01  1
#define PIN02  2
#define PIN03  3
#define PIN04  4
#define PIN05  5
#define PIN06  6
#define PIN07  7
#define PIN08  8
#define PIN09  9
#define PINCLK 10
#define PWM1   11
#define PWM2   12

ProcessorState ps = ProcessorState::DOWN;
BatteryState   bs = BatteryState::DEAD_FLAT;
PanelState     cs = PanelState::NIGHT;

void i2creceive(int n);
void setBattery();
void setPanel();
extern void setI2C();

//------------------------------------------------------------------------------
void
setup()
  {
  pinMode(PIN00,  INPUT_PULLUP);
  pinMode(PIN01,  INPUT_PULLUP);
  pinMode(PIN02,  INPUT_PULLUP);
  pinMode(PIN03,  INPUT_PULLUP);
  pinMode(PIN04,  INPUT_PULLUP);
  pinMode(PIN05,  INPUT_PULLUP);
  pinMode(PIN06,  INPUT_PULLUP);
  pinMode(PIN07,  INPUT_PULLUP);
  pinMode(PIN08,  INPUT_PULLUP);
  pinMode(PIN09,  INPUT_PULLUP);
  pinMode(PINCLK, INPUT_PULLUP);
  setBattery();
  setPanel();
  setI2C();
  Wire.onReceive(i2creceive);
  }

//------------------------------------------------------------------------------
void
loop()
  {
  if (digitalRead(PINCLK) == LOW)
    {   // Don't bother to debounce.
    ProcessorState psc;
    BatteryState   bsc;
    PanelState     csc;
    psc = ProcessorState(digitalRead(PIN00) * 1 + 
                         digitalRead(PIN01) * 2 +
                         digitalRead(PIN02) * 4);
    bsc = BatteryState(digitalRead(PIN03) * 1 +
                       digitalRead(PIN04) * 2 +
                       digitalRead(PIN05) * 4);
    csc = PanelState(digitalRead(PIN06) * 1);
    if (bsc != bs || csc != cs)
      {
      bs = bsc;
      cs = csc;
      setBattery();
      setPanel();
      }
    ps = psc;
    }
  }

//------------------------------------------------------------------------------
void
i2creceive(int n)
  {
  if (Wire.available())
    if (Wire.read() == 0);  // Should be register 0;
      Wire.write(byte(ps));    // Simulate processor status.
  }

//------------------------------------------------------------------------------
void
setBattery()
  {
  switch (bs)
    {
    case BatteryState::DEAD_FLAT:
      analogWrite(PWM2, 700 / 4);
      break;
    case BatteryState::FLAT:
      analogWrite(PWM2, 735 / 4);
      break;
    case BatteryState::DISCHARGED:
      analogWrite(PWM2, 765 / 4);
      break;
    case BatteryState::REDUCED:
      analogWrite(PWM2, 790 / 4);
      break;
    case BatteryState::PARTIAL:
      analogWrite(PWM2, 815 / 4);
      break;
    case BatteryState::NOMINAL:
      analogWrite(PWM2, 835 / 4);
      break;
    case BatteryState::FULL:
      analogWrite(PWM2, 860 / 4);
      break;
    case BatteryState::OVER:
      analogWrite(PWM2, 900 / 4);
      break;
    }
  }

//------------------------------------------------------------------------------
void
setPanel()
  {
  switch (cs)
    {
    case PanelState::NIGHT:
      analogWrite(PWM1, 60 / 4);
      break;
    case PanelState::DAY:
      analogWrite(PWM1, 420 / 4);
      break;
    }
  }
