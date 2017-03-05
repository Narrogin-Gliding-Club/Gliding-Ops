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

#include "Arduino.h"
#include "MsTimer2.h"
#include "I2Cdev.h"
#include "LowPower.h"

#include <avr/pgmspace.h>
#include <Wire.h>

#define LED_BUILTIN    13
#define K1             12
#define K2             11
#define K3             10
#define K4              9
#define SHUTDOWNPIN     8
#define I2C_ADSL_ADDR  64
#define I2C_FLARM_ADDR 65

#define BAT_SETPOINT_1 718  // 11.0V
#define BAT_SETPOINT_2 750  // 11.5V
#define BAT_SETPOINT_3 783  // 12.0V
#define BAT_SETPOINT_4 802  // 12.3V
#define BAT_SETPOINT_5 822  // 12.6V
#define BAT_SETPOINT_6 848  // 13.0V
#define BAT_SETPOINT_7 881  // 13.5V
#define PAN_SETPOINT_1  62  // 1.5V
#define PAN_SETPOINT_2 416  // 10.0V

enum class PollResponse : byte
  {
  RUNNING,
  IDLE,
  SHUTINGDOWN,
  };

enum class PollCommand : byte
  {
  NONE,
  SHUTDOWNFLARM,
  ENABLEFLARM,    // Signal that FLARM may be turned on today.
  SHUTDOWNADSL,
  };

enum class Command : byte
  {
  NONE,
  DOWN,
  KILL_APP,
  LAUNCH_APP,
  };

enum class ProcessorState : byte
  {
  UP,
  IDLE,
  BOOTING,
  SHUTINGDOWN,
  DOWN,
  };

enum class BatteryState : byte
  {
  CHARGING,
  CHARGED,
  NORMAL,
  REDUCED,
  DISCHARGED,
  DEAD_FLAT,
  };

enum class PanelState : byte
  {
  DAY,
  NIGHT,
  };

ProcessorState processor_state[3] = {
                                    ProcessorState::DOWN,
                                    ProcessorState::DOWN,
                                    ProcessorState::UP
                                    };

/**
 Global common.
 */
void dispatch();
void i2creceive(int n);
bool poll(byte addr, volatile PollResponse *r);
void extract_command(byte addr, byte *c);
bool command(byte addr, Command c);
void update_processor_status(byte p, volatile PollResponse b);

// Global common.
BatteryState   battery_state = BatteryState::DISCHARGED;// Until we know better.
PanelState     panel_state   = PanelState::NIGHT;       // Until we know better.
int tick = 0;
volatile int adsl_to  = 0;
volatile int flarm_to = 0;
volatile int bat_v    = BAT_SETPOINT_7;  // Until the first A/D conversion.
volatile int pan_v    = 0;
volatile PollResponse adsl_response;
volatile PollResponse flarm_response;
byte led_state = 0;

//------------------------------------------------------------------------------
void
setup()
  {
  pinMode(K1, OUTPUT);          // Relay
  pinMode(K2, OUTPUT);
  pinMode(K3, OUTPUT);
  pinMode(K4, OUTPUT);
  digitalWrite(K1, LOW);        // Turn this device on!
  digitalWrite(K2, HIGH);       // Turn everything else off!
  digitalWrite(K3, HIGH);
  digitalWrite(K4, HIGH);
  Serial.begin(115200);         // For debug.
  pinMode(LED_BUILTIN, OUTPUT); // Pin 13 as a LED display (yellow).
  pinMode(SHUTDOWNPIN, INPUT_PULLUP);
  Wire.begin();                 // join i2c bus (address optional for master)
  Wire.onReceive(i2creceive);   // register receive event
  tick = 0;
  delay(10000);                 // Allow a serial connection.
  }

//------------------------------------------------------------------------------
void
loop()
  {
  if (tick % 2 == 0)
    { // Every 30 mSec
    if ((digitalRead(SHUTDOWNPIN) == LOW) &&
        (processor_state[2] != ProcessorState::SHUTINGDOWN))
      { // Do not bother to debounce
      if (Serial)
        Serial.println(SHUTDOWNPIN);
      processor_state[2] = ProcessorState::SHUTINGDOWN; // This process is
                                                        // shuting down.
      command(I2C_FLARM_ADDR, Command::DOWN);
      command(I2C_ADSL_ADDR,  Command::DOWN);
      }
    }

  if (tick % 4 == 0)
    { // Every 60 mSec
    }

  if (tick % 8 == 0)
    { // Every 120 mSec
    }

  if ( tick % 32 == 0)
    { // Every 480 mSec
    if (led_state == 0)
      {
      digitalWrite(LED_BUILTIN, HIGH);
      led_state = 1;
      }
    else
      {
      digitalWrite(LED_BUILTIN, LOW);
      led_state = 0;
      }
    }

  if (tick % 64 == 0)
    { // Every 960 mSec

    if (adsl_to > 100)
      {
      processor_state[0] = ProcessorState::DOWN;
      adsl_to = 0;
      if (Serial)
        Serial.println("0: DOWN");
      }
    if (flarm_to > 100)
      {
      processor_state[1] = ProcessorState::DOWN;
      flarm_to = 0;
      if (Serial)
        Serial.println("1: DOWN");
      }

    if (processor_state[0] == ProcessorState::UP      ||
        processor_state[0] == ProcessorState::IDLE    ||
        processor_state[0] == ProcessorState::BOOTING ||
        processor_state[0] == ProcessorState::SHUTINGDOWN)
      {
      if (poll(I2C_ADSL_ADDR, &adsl_response) == true)
        {
        adsl_to = 0;
        update_processor_status(0, adsl_response);
        }
      else
        adsl_to++;
      }
    if (processor_state[1] == ProcessorState::UP      ||
        processor_state[1] == ProcessorState::IDLE    ||
        processor_state[1] == ProcessorState::BOOTING ||
        processor_state[1] == ProcessorState::SHUTINGDOWN)
      {
      if (poll(I2C_FLARM_ADDR, &flarm_response) == true)
        {
        flarm_to = 0;
        update_processor_status(1, flarm_response);
        }
      else
        flarm_to++;
      }
    }
  
  if (tick % 1024 == 0)
    { // Every 15.360 Sec

    // Read the battery and panel.
    bat_v = analogRead(5);
    pan_v = analogRead(4);

    if (Serial)
      {
      Serial.print("B: ");
      Serial.println(bat_v, DEC);
      Serial.print("P: ");
      Serial.println(pan_v, DEC);
      }
    switch (processor_state[0])
      {
      case ProcessorState::DOWN:
        if (bat_v > BAT_SETPOINT_5)
          {
          digitalWrite(K2, LOW);  // Powerup adsl.
          processor_state[0] = ProcessorState::BOOTING;
          if (Serial)
            Serial.println("0: BOOTING");
          }
        break;
      case ProcessorState::BOOTING:
        if (bat_v < BAT_SETPOINT_2)
          {
          digitalWrite(K2, HIGH); // Not much else we can do here.
          processor_state[0] = ProcessorState::DOWN;
          if (Serial)
            Serial.println("0: DOWN");
          }
        break;
      case ProcessorState::UP:
        if (bat_v < BAT_SETPOINT_3)
          command(I2C_ADSL_ADDR, Command::KILL_APP);
        if (bat_v < BAT_SETPOINT_2)
          {
          command(I2C_ADSL_ADDR, Command::DOWN);
          processor_state[0] = ProcessorState::SHUTINGDOWN;
          if (Serial)
            Serial.println("0: SHUTINGDOWN");
          }
        if (bat_v < BAT_SETPOINT_1)
          {
          if (Serial)
            Serial.println("2: DEAD FLAT");
          digitalWrite(K3, HIGH); // This will certainly already be DOWN.
          digitalWrite(K2, HIGH); // Likewise, this.
          digitalWrite(K1, HIGH); // This kills everything including the
                                  // processor on which this program runs.
          }
        break;
      case ProcessorState::IDLE:
        if (bat_v > BAT_SETPOINT_4)
          command(I2C_ADSL_ADDR, Command::LAUNCH_APP);

        break;
      }

    switch (panel_state)
      {
      case PanelState::DAY:
        if (pan_v < PAN_SETPOINT_1)
          {
          panel_state = PanelState::NIGHT;
          if (Serial)
            Serial.println("NIGHT");
          }
        break;
      case PanelState::NIGHT:
        if (pan_v > PAN_SETPOINT_2)
          {
          panel_state = PanelState::DAY;
          if (Serial)
            Serial.println("DAY");
          }
        break;
      }

    switch (processor_state[1])
      {
      case ProcessorState::DOWN:
        if ((panel_state == PanelState::DAY) && (bat_v > BAT_SETPOINT_6))
          {
          digitalWrite(K3, LOW);
          processor_state[1] = ProcessorState::BOOTING;
          if (Serial)
            Serial.println("1: BOOTING");
          }
        break;
      case ProcessorState::BOOTING:
        if (bat_v < BAT_SETPOINT_4)
          {
          digitalWrite(K3, HIGH); // Not much else we can do here.
          processor_state[1] = ProcessorState::DOWN;
          if (Serial)
            Serial.println("1: DOWN");
          }
        break;
      case ProcessorState::UP:
        if (panel_state == PanelState::NIGHT)
          {
          command(I2C_FLARM_ADDR, Command::DOWN);
          processor_state[1] = ProcessorState::SHUTINGDOWN;
          if (Serial)
            Serial.println("1: SHUTINGDOWN");
          }
        else if (bat_v < BAT_SETPOINT_5)
          command(I2C_FLARM_ADDR, Command::KILL_APP);
        else if (bat_v < BAT_SETPOINT_4);
          {
          command(I2C_FLARM_ADDR, Command::DOWN);
          processor_state[1] = ProcessorState::SHUTINGDOWN;
          if (Serial)
            Serial.println("1: SHUTINGDOWN");
          }
        break;
      case ProcessorState::IDLE:
        if (panel_state == PanelState::NIGHT)
          {
          command(I2C_FLARM_ADDR, Command::DOWN);
          processor_state[1] = ProcessorState::SHUTINGDOWN;
          if (Serial)
            Serial.println("1: SHUTINGDOWN");
          }
        else if (bat_v > BAT_SETPOINT_6)
          command(I2C_FLARM_ADDR, Command::LAUNCH_APP);
        break;
      }

    tick = 0; // Reset at the longest time interval to prevent overflow.
    }
  tick++;

  if (!Serial)
    {
    // Go to a low power mode for a while.
    LowPower.idle(SLEEP_15MS,
                  ADC_OFF,
                  TIMER4_OFF,
                  TIMER3_OFF,
                  TIMER1_OFF,
                  TIMER0_OFF,
                  SPI_OFF,
                  USART1_OFF,
                  TWI_OFF,
                  USB_OFF);
    }
  else
    delay(15);
  }

//------------------------------------------------------------------------------
void
i2creceive(int n)
  {
  }

//------------------------------------------------------------------------------
bool
poll(byte addr, volatile PollResponse *r)
  {
  Wire.beginTransmission(addr);
  Wire.write("p");
  Wire.endTransmission();
  Wire.requestFrom(addr, 1, false);
  if (Wire.available() > 0)
    {
    *r = (PollResponse )Wire.read();
    return true;
    }
  else
    return false;
  }

//------------------------------------------------------------------------------
bool
command(byte addr, Command c)
  {
  Wire.beginTransmission(addr);
  Wire.write("c");
  Wire.write((byte )c);
  Wire.endTransmission();
  return true;
  }

//------------------------------------------------------------------------------
void
update_processor_status(byte p, volatile PollResponse r)
  {
  switch (r)
    {
    case PollResponse::SHUTINGDOWN:
      processor_state[p] = ProcessorState::SHUTINGDOWN;
      if (Serial)
        {
        Serial.print(p, DEC);
        Serial.println(": SHUTINGDOWN");
        }
      break;
    case PollResponse::RUNNING:
      processor_state[p] = ProcessorState::UP;
      if (Serial)
        {
        Serial.print(p, DEC);
        Serial.println(": UP");
        }
      break;
    case PollResponse::IDLE:
      processor_state[p] = ProcessorState::IDLE;
      if (Serial)
        {
        Serial.print(p, DEC);
        Serial.println(": IDLE");
        }
      break;
    }
  }
