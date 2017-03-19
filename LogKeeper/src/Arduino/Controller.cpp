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

#define LED_BUILTIN    13
#define K1             12
#define K2             11
#define K3             10
#define K4              9
#define P0UP            0
#define P0APP           1
#define P1UP            4
#define P1APP           5
#define SHUTDOWNPIN     8
#define I2C_ADSL_ADDR  64
#define I2C_FLARM_ADDR 65
#define ADSL            0
#define FLARM           1

#define BAT_SETPOINT_1 uint16_t(718)  // 11.0V
#define BAT_SETPOINT_2 uint16_t(750)  // 11.5V
#define BAT_SETPOINT_3 uint16_t(783)  // 12.0V
#define BAT_SETPOINT_4 uint16_t(802)  // 12.3V
#define BAT_SETPOINT_5 uint16_t(822)  // 12.6V
#define BAT_SETPOINT_6 uint16_t(848)  // 13.0V
#define BAT_SETPOINT_7 uint16_t(881)  // 13.5V
#define PAN_SETPOINT_1 uint16_t( 62)  // 1.5V
#define PAN_SETPOINT_2 uint16_t(416)  // 10.0V


/**
 Global common.
 */
void i2creceive(int n);

// Global common.
BatteryState   battery_state = BatteryState::FLAT;  // Until we know better.
PanelState     panel_state   = PanelState::NIGHT;   // Until we know better.
uint16_t tick = 0;
volatile uint16_t bat_v   = BAT_SETPOINT_7;  // Until the first A/D conversion.
volatile uint16_t pan_v   = 0;
uint16_t bat_acc;
uint16_t pan_acc;
uint8_t  nreadings;
uint8_t  i2cbuffer[64];

Arduino arduino;
Adsl    adsl;
Flarm   flarm;
TwoWire WSWire;

//------------------------------------------------------------------------------
void
setup()
  {
  Serial.begin(uint32_t(115200));// For debug.
  pinMode(LED_BUILTIN, OUTPUT); // Pin 13 as a LED display (yellow).
  pinMode(SHUTDOWNPIN, INPUT_PULLUP);
  WSWire.begin(32);             // join i2c bus master and slave 32. Not used
                                // at this time.
  WSWire.onReceive(i2creceive); // register receive event
  tick = 0;
  nreadings = 0;
  delay(10000);                 // Allow a serial connection.
  }

//------------------------------------------------------------------------------
void
loop()
  {
  if ((tick % 2) == 0)
    { // Every 30 mSec
    arduino.Tick2();
    adsl.Tick2();
    flarm.Tick2();
    }

  if ((tick % 4) == 0)
    { // Every 60 mSec
    arduino.Tick4();
    adsl.Tick4();
    flarm.Tick4();
    }

  if ((tick % 8) == 0)
    { // Every 120 mSec
    arduino.Tick8();
    adsl.Tick8();
    flarm.Tick8();
    }

  if ((tick % 32) == 0)
    { // Every 480 mSec
    arduino.Tick32();
    adsl.Tick32();
    flarm.Tick32();
    }

  if ((tick % 64) == 0)
    { // Every 960 mSec
    arduino.Tick64();
    adsl.Tick64();
    flarm.Tick64();
    }
  
  if ((tick % 1024) == 0)
    { // Every 15.360 Sec
    arduino.Tick1024();
    adsl.Tick1024();
    flarm.Tick1024();

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
  uint8_t from;

  if (Serial)
    {
    Serial.print("i2creceive(");
    Serial.print(n, DEC);
    Serial.println(")");
    }
  if (n == 3)
    { // If not 3 then a problem?
    if (WSWire.available())
      {
      from = WSWire.read() - 64;
      if (from < 2)
        {
        if (WSWire.available())
          i2cbuffer[0] = WSWire.read();
        if (WSWire.available())
          i2cbuffer[1] = WSWire.read();
        }
      }
    }
  }

//------------------------------------------------------------------------------
bool
poll(byte addr)
  {
#ifdef USE_I2C
  return i2c.writeByte(addr, 0, NULL, 20);
#else
  WSWire.beginTransmission(addr);
  WSWire.write((uint8_t )0);
  return (WSWire.endTransmission() == 0) ? true : false;
#endif
  }

//------------------------------------------------------------------------------
Processor::Processor()
  {
  this->power_off_timer = 0;
  }

//------------------------------------------------------------------------------
Processor::~Processor()
  {
  }

//------------------------------------------------------------------------------
void
Processor::Tick2()
  {
  if (digitalRead(SHUTDOWNPIN) == LOW)
    { // Do not bother to debounce
    this->ShutDown();
    }
  }

//------------------------------------------------------------------------------
void
Processor::Tick4()
  {
  }

//------------------------------------------------------------------------------
void
Processor::Tick8()
  {
  }

//------------------------------------------------------------------------------
void
Processor::Tick32()
  {
  }

//------------------------------------------------------------------------------
void
Processor::Tick64()
  {
  if (this->state == ProcessorState::SHUTTINGDOWN)
    {
    if (this->power_off_timer > 128)
      this->PowerOff();
    else
      this->power_off_timer++;
    }
  else
    this->power_off_timer = 0;
  }

//------------------------------------------------------------------------------
void
Processor::Tick1024()
  {
  }

//------------------------------------------------------------------------------
ProcessorState
Processor::State() const
  {
  return this->state;
  }

//------------------------------------------------------------------------------
void
Processor::RunApp()
  {
  }

//------------------------------------------------------------------------------
void
Processor::KillApp()
  {
  }

//------------------------------------------------------------------------------
Adsl::Adsl()
  : Processor()
  {
  pinMode(K2,    OUTPUT);
  pinMode(P0UP,  OUTPUT);
  pinMode(P0APP, OUTPUT);
  this->PowerOff();
  }

//------------------------------------------------------------------------------
Adsl::~Adsl()
  {
  this->PowerOff();
  }

//------------------------------------------------------------------------------
void
Adsl::Tick1024()
  {
  Processor::Tick1024();

  switch (this->state)
    {
    case ProcessorState::POWER_OFF:
      if (::bat_v > BAT_SETPOINT_4)
        this->PowerOn();
      break;
    case ProcessorState::RUNNING:
      if (::bat_v < BAT_SETPOINT_3)
        this->KillApp();
      if (::bat_v < BAT_SETPOINT_2)
        this->ShutDown();
      break;
    case ProcessorState::IDLE:
      if (::bat_v > BAT_SETPOINT_5)
        this->RunApp();
      break;
    default:
      break;
    }
  }

//------------------------------------------------------------------------------
void 
Adsl::KillApp()
  {
  digitalWrite(P0APP, HIGH);
  this->state = ProcessorState::IDLE;
  }

//------------------------------------------------------------------------------
void
Adsl::RunApp()
  {
  digitalWrite(P0APP, LOW);
  this->state = ProcessorState::RUNNING;
  }

//------------------------------------------------------------------------------
void
Adsl::PowerOff()
  {
  digitalWrite(K2, HIGH);
  this->state = ProcessorState::POWER_OFF;
  }

//------------------------------------------------------------------------------
void
Adsl::PowerOn()
  {
  this->KillApp();
  digitalWrite(P0UP, LOW);
  digitalWrite(P0APP, HIGH);
  digitalWrite(K2, LOW);
  }

//------------------------------------------------------------------------------
void
Adsl::ShutDown()
  {
  digitalWrite(P0UP, HIGH);
  this->state = ProcessorState::SHUTTINGDOWN;
  }

//------------------------------------------------------------------------------
Flarm::Flarm()
  : Processor()
  {
  pinMode(K3,    OUTPUT);
  pinMode(K4,    OUTPUT);
  pinMode(P1UP,  OUTPUT);
  pinMode(P1APP, OUTPUT);
  this->PowerOff();
  }

//------------------------------------------------------------------------------
Flarm::~Flarm()
  {
  }

//------------------------------------------------------------------------------
void
Flarm::Tick1024()
  {
  Processor::Tick1024();

  switch (this->state)
    {
    case ProcessorState::POWER_OFF:
      if ((::panel_state == PanelState::DAY) && (::bat_v > BAT_SETPOINT_6))
        this->PowerOn();
      break;
    case ProcessorState::DOWN:
      this->PowerOff(); // Its not running so power it off.
      break;
    case ProcessorState::BOOTING:
      if (::bat_v < BAT_SETPOINT_4)
        this->ShutDown(); // Not much else we can do here.
      break;
    case ProcessorState::RUNNING:
      if (::panel_state == PanelState::NIGHT)
        this->ShutDown();
      else
        {
        if (::bat_v < BAT_SETPOINT_4)
          this->ShutDown();
        else
          {
          if (::bat_v < BAT_SETPOINT_5)
            this->KillApp();
          }
        }
      break;
    case ProcessorState::IDLE:
      if (::panel_state == PanelState::NIGHT)
        this->ShutDown();
      else
        {
        if (::bat_v > BAT_SETPOINT_6)
          this->RunApp();
        }
      break;
    default:
      break;
    }
  }

//------------------------------------------------------------------------------
void
Flarm::KillApp()
  {
  digitalWrite(P1APP, HIGH);
  this->state = ProcessorState::IDLE;
  }

//------------------------------------------------------------------------------
void
Flarm::RunApp()
  {
  digitalWrite(P1APP, LOW);
  this->state = ProcessorState::RUNNING;
  }

//------------------------------------------------------------------------------
void
Flarm::PowerOff()
  {
  digitalWrite(K4, HIGH);
  digitalWrite(K3, HIGH);
  this->state = ProcessorState::POWER_OFF;
  }

//------------------------------------------------------------------------------
void
Flarm::PowerOn()
  {
  this->KillApp();
  digitalWrite(P1UP, LOW);
  digitalWrite(P1APP, HIGH);
  digitalWrite(K3, LOW);
  }

//------------------------------------------------------------------------------
void
Flarm::ShutDown()
  {
  digitalWrite(P1UP, HIGH);
  this->state = ProcessorState::SHUTTINGDOWN;
  }

//------------------------------------------------------------------------------
Arduino::Arduino()
  : Processor()
  {
  this->PowerOn();
  ::bat_acc = ::pan_acc = 0;
  ::nreadings = 0;
  this->led_state = 0;
  }

//------------------------------------------------------------------------------
Arduino::~Arduino()
  {
  }

//------------------------------------------------------------------------------
void
Arduino::Tick8()
  {
  if (this->state == ProcessorState::SHUTTINGDOWN)
    {
    if (led_state == 0)
      {
      digitalWrite(LED_BUILTIN, HIGH);
      this->led_state = 1;
      }
    else
      {
      digitalWrite(LED_BUILTIN, LOW);
      this->led_state = 0;
      }
    }
  }

//------------------------------------------------------------------------------
void
Arduino::Tick32()
  {
  if (this->state != ProcessorState::SHUTTINGDOWN)
    {
    if (led_state == 0)
      {
      digitalWrite(LED_BUILTIN, HIGH);
      this->led_state = 1;
      }
    else
      {
      digitalWrite(LED_BUILTIN, LOW);
      this->led_state = 0;
      }
    }
  }

//------------------------------------------------------------------------------
void
Arduino::Tick64()
  {
  ::bat_acc += analogRead(5);
  ::pan_acc += analogRead(4);
  ::nreadings++;
  }

//------------------------------------------------------------------------------
void
Arduino::Tick1024()
  {
  ::bat_v = bat_acc / nreadings;
  ::pan_v = pan_acc / nreadings;
  ::bat_acc = 0;
  ::pan_acc = 0;
  ::nreadings = 0;
  if (Serial)
    {
    Serial.print("B: ");
    Serial.println(::bat_v, DEC);
    Serial.print("P: ");
    Serial.println(::pan_v, DEC);
    }

  if (::bat_v < BAT_SETPOINT_1)
    {
    if (Serial)
      Serial.println("2: DEAD FLAT");
    digitalWrite(K4, HIGH);
    digitalWrite(K3, HIGH); // This will certainly already be DOWN.
    digitalWrite(K2, HIGH); // Likewise, this.
    digitalWrite(K1, HIGH); // This kills everything including the
                            // processor on which this program runs.
    Serial.println("0, 1, 2: POWER OFF"); // Only get here of powered
                                          // by serial.
    }

  switch (panel_state)
    {
    case PanelState::DAY:
      if (::pan_v < PAN_SETPOINT_1)
        {
        ::panel_state = PanelState::NIGHT;
        if (Serial)
          Serial.println("NIGHT");
        }
      break;
    case PanelState::NIGHT:
      if (::pan_v > PAN_SETPOINT_2)
        {
        ::panel_state = PanelState::DAY;
        if (Serial)
          Serial.println("DAY");
        }
      break;
    }
  }

//------------------------------------------------------------------------------
void
Arduino::PowerOff()
  {
  digitalWrite(K1, HIGH);
  this->state = ProcessorState::POWER_OFF;
  }

//------------------------------------------------------------------------------
void
Arduino::PowerOn()
  {
  digitalWrite(K1, LOW);
  this->state = ProcessorState::RUNNING;
  }

//------------------------------------------------------------------------------
void
Arduino::ShutDown()
  {
  this->state = ProcessorState::SHUTTINGDOWN;
  }
