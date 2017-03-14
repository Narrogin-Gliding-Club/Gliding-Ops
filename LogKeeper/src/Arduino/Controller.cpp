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
#include "LowPower.h"

#include <avr/pgmspace.h>
#include <WSWire.h>

#include "ControllerStates.hpp"

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
bool poll(byte addr, volatile Reg0Response *r);
void extract_command(byte addr, byte *c);
bool command(byte addr, Command c);
void update_processor_status(byte p, volatile Reg0Response b);

// Global common.
BatteryState   battery_state = BatteryState::FLAT;      // Until we know better.
PanelState     panel_state   = PanelState::NIGHT;       // Until we know better.
int tick = 0;
volatile int adsl_to  = 0;
volatile int flarm_to = 0;
volatile int bat_v    = BAT_SETPOINT_7;  // Until the first A/D conversion.
volatile int pan_v    = 0;
volatile Reg0Response adsl_response;
volatile Reg0Response flarm_response;
byte led_state = 0;
uint16_t bat_acc;
uint16_t pan_acc;
uint8_t  nreadings;
TwoWire WSWire;

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
  WSWire.begin();                 // join i2c bus (address optional for master)
  WSWire.onReceive(i2creceive);   // register receive event
  tick = 0;
  nreadings = 0;
  delay(10000);                 // Allow a serial connection.
  }

//------------------------------------------------------------------------------
void
loop()
  {
  if (tick % 2 == 0)
    { // Every 30 mSec
    if ((digitalRead(SHUTDOWNPIN) == LOW) &&
        (processor_state[2] != ProcessorState::SHUTTINGDOWN))
      { // Do not bother to debounce
      processor_state[2] = ProcessorState::SHUTTINGDOWN; // This process is
                                                        // shuting down.
      if (Serial)
        Serial.println("2: SHUTTINGDOWN");
      command(I2C_FLARM_ADDR, Command::DOWN);
      command(I2C_ADSL_ADDR,  Command::DOWN);
      }
    }

  if (tick % 4 == 0)
    { // Every 60 mSec
    }

  if (tick % 8 == 0)
    { // Every 120 mSec
    if (processor_state[2] == ProcessorState::SHUTTINGDOWN)
      {
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
    }

  if ( tick % 32 == 0)
    { // Every 480 mSec
    if (processor_state[2] != ProcessorState::SHUTTINGDOWN)
      {
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
    }

  if (tick % 64 == 0)
    { // Every 960 mSec

    bat_acc += analogRead(5);
    pan_acc += analogRead(4);
    nreadings++;

    if (adsl_to > 100)
      {
      processor_state[0] = ProcessorState::DOWN;
      digitalWrite(K2, HIGH); // Ensure power is remove if not already.
      adsl_to = 0;
      if (Serial)
        Serial.println("0: DOWN");
      }
    if (flarm_to > 100)
      {
      processor_state[1] = ProcessorState::DOWN;
      digitalWrite(K2, HIGH); // Ensure power is remove if not already.
      flarm_to = 0;
      if (Serial)
        Serial.println("1: DOWN");
      }

    if (processor_state[0] == ProcessorState::UP      ||
        processor_state[0] == ProcessorState::IDLE    ||
        processor_state[0] == ProcessorState::BOOTING ||
        processor_state[0] == ProcessorState::SHUTTINGDOWN)
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
        processor_state[1] == ProcessorState::SHUTTINGDOWN)
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

    if (nreadings >= 4)
      {
      bat_v = bat_acc / nreadings;
      pan_v = pan_acc / nreadings;
      bat_acc = 0;
      pan_acc = 0;
      nreadings = 0;
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
            processor_state[0] = ProcessorState::SHUTTINGDOWN;
            if (Serial)
              Serial.println("0: SHUTTINGDOWN");
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
            processor_state[1] = ProcessorState::SHUTTINGDOWN;
            if (Serial)
              Serial.println("1: SHUTTINGDOWN");
            }
          else if (bat_v < BAT_SETPOINT_5)
            command(I2C_FLARM_ADDR, Command::KILL_APP);
          else if (bat_v < BAT_SETPOINT_4);
            {
            command(I2C_FLARM_ADDR, Command::DOWN);
            processor_state[1] = ProcessorState::SHUTTINGDOWN;
            if (Serial)
              Serial.println("1: SHUTTINGDOWN");
            }
          break;
        case ProcessorState::IDLE:
          if (panel_state == PanelState::NIGHT)
            {
            command(I2C_FLARM_ADDR, Command::DOWN);
            processor_state[1] = ProcessorState::SHUTTINGDOWN;
            if (Serial)
              Serial.println("1: SHUTTINGDOWN");
            }
          else if (bat_v > BAT_SETPOINT_6)
            command(I2C_FLARM_ADDR, Command::LAUNCH_APP);
          break;
        }
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
poll(byte addr, volatile Reg0Response *r)
  {
  bool rtn = false;
  if (Serial)
    {
    Serial.print("poll ");
    Serial.println(addr, DEC);
    }
#ifdef USE_I2C
  rtn = i2c.readByte(addr, 0, (byte *)r, 20);
#else
  WSWire.beginTransmission(addr);
  WSWire.write((uint8_t )0);
  WSWire.endTransmission();
  WSWire.beginTransmission(addr);
  WSWire.requestFrom(addr, 1);
  if (WSWire.available())
    {
    *r = Reg0Response(WSWire.read());
    rtn = true;
    }
  else
    rtn = false;
#endif
  Serial.println("poll return");
  return rtn;
  }

//------------------------------------------------------------------------------
bool
command(byte addr, Command c)
  {
#ifdef USE_I2C
  return i2c.writeByte(addr, 0, (byte )c);
#else
  WSWire.beginTransmission(addr);
  WSWire.write((uint8_t )0);
  WSWire.endTransmission();
  WSWire.beginTransmission(addr);
  WSWire.write((byte )c);
  return (WSWire.endTransmission() == 0) ? true : false;
#endif
  }

//------------------------------------------------------------------------------
void
update_processor_status(byte p, volatile Reg0Response r)
  {
  switch (r)
    {
    case Reg0Response::SHUTTINGDOWN:
      processor_state[p] = ProcessorState::SHUTTINGDOWN;
      if (Serial)
        {
        Serial.print(p, DEC);
        Serial.println(": SHUTTINGDOWN");
        }
      break;
    case Reg0Response::RUNNING:
      processor_state[p] = ProcessorState::UP;
      if (Serial)
        {
        Serial.print(p, DEC);
        Serial.println(": UP");
        }
      break;
    case Reg0Response::IDLE:
      processor_state[p] = ProcessorState::IDLE;
      if (Serial)
        {
        Serial.print(p, DEC);
        Serial.println(": IDLE");
        }
      break;
    }
  }
