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
#define K1              9
#define K2             10
#define K3             11
#define K4             12
#define P0UP            6
#define P0APP           7
#define P1UP            4
#define P1APP           5
#define SHUTDOWNPIN     8
#define I2C_ADSL_ADDR  64
#define I2C_FLARM_ADDR 65
#define ADSL            0
#define FLARM           1
#define ON              HIGH  // Relay activated
#define OFF             LOW   // Relay deactivated

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
volatile bool flarm_allow;
volatile bool flarm_received;

Arduino *arduino;
Adsl    *adsl;
Flarm   *flarm;
Switch  *lan;
TwoWire WSWire;

//------------------------------------------------------------------------------
void
setup()
  {
  Serial.begin(uint32_t(115200));// For debug.
  pinMode(LED_BUILTIN, OUTPUT); // Pin 13 as a LED display (yellow).
  digitalWrite(LED_BUILTIN, HIGH);
  pinMode(SHUTDOWNPIN, INPUT_PULLUP);
  ::arduino = new Arduino();
  ::adsl    = new Adsl();
  ::flarm   = new Flarm();
  ::lan     = new Switch();
  WSWire.begin(32);             // join i2c bus master and slave 32. Not used
                                // at this time.
  digitalWrite(SCL, 0);         // Turn off i2c pullups for 3v3 Pi.
  digitalWrite(SDA, 0);
  WSWire.onReceive(i2creceive); // register receive event
  ::tick           = -1023;     // Allow some voltage readings before action.
  ::nreadings      = 0;
  ::flarm_allow    = false;
  ::flarm_received = false;
  delay(10000);                 // Allow a serial connection.
  digitalWrite(LED_BUILTIN, LOW);
  }

//------------------------------------------------------------------------------
void
loop()
  {
  if ((tick % 2) == 0)
    { // Every 30 mSec
    ::arduino->Tick2();
    ::adsl->Tick2();
    ::flarm->Tick2();
    ::lan->Tick2();
    }

  if ((tick % 4) == 0)
    { // Every 60 mSec
    ::arduino->Tick4();
    ::adsl->Tick4();
    ::flarm->Tick4();
    ::lan->Tick4();
    }

  if ((tick % 8) == 0)
    { // Every 120 mSec
    ::arduino->Tick8();
    ::adsl->Tick8();
    ::flarm->Tick8();
    ::lan->Tick8();
    }

  if ((tick % 32) == 0)
    { // Every 480 mSec
    ::arduino->Tick32();
    ::adsl->Tick32();
    ::flarm->Tick32();
    ::lan->Tick32();
    }

  if ((tick % 64) == 0)
    { // Every 960 mSec
    ::arduino->Tick64();
    ::adsl->Tick64();
    ::flarm->Tick64();
    ::lan->Tick64();
    }
  
  if ((tick % 1024) == 0)
    { // Every 15.360 Sec
    ::arduino->Tick1024();
    ::adsl->Tick1024();
    ::flarm->Tick1024();
    ::lan->Tick1024();

    ::tick = 0; // Reset at the longest time interval to prevent overflow.
    }
  ::tick++;

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
  ::flarm_received = true;
  if (n == 1)
    { // If not 1 then a problem?
    if (WSWire.available())
      flarm_allow = bool(WSWire.read());
    }
  }

//------------------------------------------------------------------------------
bool
poll(byte addr)
  {
  WSWire.beginTransmission(addr);
  WSWire.write((uint8_t )0);
  return (WSWire.endTransmission() == 0) ? true : false;
  }

//------------------------------------------------------------------------------
Processor::Processor()
  {
  this->power_timer = 0;
  }

//------------------------------------------------------------------------------
Processor::~Processor()
  {
  }

//------------------------------------------------------------------------------
void
Processor::Tick2()
  {
  if ((digitalRead(SHUTDOWNPIN) == LOW) &&
      (this->state != ProcessorState::SHUTTINGDOWN))
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
    if (this->power_timer > 128)
      this->PowerOff();
    else
      this->power_timer++;
    }
  else
    this->power_timer = 0;
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
Adsl::Tick64()
  {
  Processor::Tick64();
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
      if (::bat_v < BAT_SETPOINT_2)
        this->ShutDown();
      break;
    default:
      break;
    }
  }

//------------------------------------------------------------------------------
void 
Adsl::KillApp()
  {
  if (Serial)
    Serial.println("Adsl::KillApp()");
  digitalWrite(P0APP, HIGH);
  this->state = ProcessorState::IDLE;
  }

//------------------------------------------------------------------------------
void
Adsl::RunApp()
  {
  if (Serial)
    Serial.println("Adsl::RunApp()");
  digitalWrite(P0APP, LOW);
  this->state = ProcessorState::RUNNING;
  }

//------------------------------------------------------------------------------
void
Adsl::PowerOff()
  {
  if (Serial)
    Serial.println("Adsl::PowerOff()");
  digitalWrite(K2,    OFF);
  digitalWrite(P0UP,  OFF);
  digitalWrite(P0APP, OFF);
  this->state = ProcessorState::POWER_OFF;
  }

//------------------------------------------------------------------------------
void
Adsl::PowerOn()
  {
  this->KillApp();
  if (Serial)
    Serial.println("Adsl::PowerOn()");
  digitalWrite(P0UP,  LOW);
  digitalWrite(P0APP, HIGH);
  digitalWrite(K2,    ON);
  }

//------------------------------------------------------------------------------
void
Adsl::ShutDown()
  {
  if (Serial)
    Serial.println("Adsl::ShutDown()");
  this->KillApp();
  digitalWrite(P0UP, HIGH);
  this->state = ProcessorState::SHUTTINGDOWN;
  }

//------------------------------------------------------------------------------
Flarm::Flarm()
  : Processor()
  {
  pinMode(K3,    OUTPUT);
  pinMode(P1UP,  OUTPUT);
  pinMode(P1APP, OUTPUT);
  this->PowerOff();
  }

//------------------------------------------------------------------------------
Flarm::~Flarm()
  {
  this->PowerOff();
  }

//------------------------------------------------------------------------------
void
Flarm::Tick64()
  {
  Processor::Tick64();
  }

//------------------------------------------------------------------------------
void
Flarm::Tick1024()
  {
  Processor::Tick1024();

  switch (this->state)
    {
    case ProcessorState::POWER_OFF:
      if ((::panel_state == PanelState::DAY) &&
          (::bat_v > BAT_SETPOINT_6)         &&
          (::flarm_allow == true))
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
        if (::bat_v < BAT_SETPOINT_4)
          this->ShutDown();
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
  if (Serial)
    Serial.println("Flarm::KillApp()");
  digitalWrite(P1APP, HIGH);
  this->state = ProcessorState::IDLE;
  }

//------------------------------------------------------------------------------
void
Flarm::RunApp()
  {
  if (Serial)
    Serial.println("Flarm::RunApp()");
  digitalWrite(P1APP, LOW);
  this->state = ProcessorState::RUNNING;
  }

//------------------------------------------------------------------------------
void
Flarm::PowerOff()
  {
  if (Serial)
    Serial.println("Flarm::PowerOff()");
  digitalWrite(K3,    OFF);
  digitalWrite(P1UP,  HIGH);
  digitalWrite(P1APP, HIGH);
  this->state = ProcessorState::POWER_OFF;
  }

//------------------------------------------------------------------------------
void
Flarm::PowerOn()
  {
  if (Serial)
    Serial.println("Flarm::PowerOn()");
  this->KillApp();
  digitalWrite(P1UP,  LOW);
  digitalWrite(P1APP, HIGH);
  digitalWrite(K3,    ON);
  }

//------------------------------------------------------------------------------
void
Flarm::ShutDown()
  {
  if (Serial)
    Serial.println("Flarm::ShutDown()");
  this->KillApp();
  digitalWrite(P1UP, HIGH);
  this->state = ProcessorState::SHUTTINGDOWN;
  }

//------------------------------------------------------------------------------
Arduino::Arduino()
  : Processor()
  {
  pinMode(K1, OUTPUT);
  this->PowerOn();
  ::bat_acc = ::pan_acc = 0;
  ::nreadings = 0;
  this->led_state = 0;
  }

//------------------------------------------------------------------------------
Arduino::~Arduino()
  {
  this->PowerOff();
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
  Processor::Tick64();
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
    Serial.print(::bat_v, DEC);
    Serial.print(" (");
    Serial.print(float(::bat_v) / 1023 * 5 * (47 + 22) / 22, 2);
    Serial.print("V), ");
    Serial.print("P: ");
    Serial.print(::pan_v, DEC);
    Serial.print(" (");
    Serial.print(float(::pan_v) / 1023 * 5 * (47 + 12) / 12, 2);
    Serial.print("V), ");
    Serial.print("I: ");
    Serial.print((::flarm_received == true) ? "true" : "false");
    Serial.print(", F: ");
    Serial.print((::flarm_allow == true) ? "true" : "false");
    Serial.println();
    }
  ::flarm_received = false;

  if ((::bat_v > BAT_SETPOINT_1) && (this->state == ProcessorState::POWER_OFF))
    this->PowerOn();    // Tautology

  if (::bat_v < BAT_SETPOINT_1)
    {
    if (Serial)
      Serial.println("2: DEAD FLAT");
    digitalWrite(K4, OFF);
    digitalWrite(K3, OFF);
    digitalWrite(K2, OFF);
    this->PowerOff();
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
  if (Serial)
    Serial.println("Arduino::PowerOff()");
  digitalWrite(K1, OFF);
  this->state = ProcessorState::POWER_OFF;
  }

//------------------------------------------------------------------------------
void
Arduino::PowerOn()
  {
  if (Serial)
    Serial.println("Arduino::PowerOn()");
  digitalWrite(K1, ON);
  this->state = ProcessorState::RUNNING;
  }

//------------------------------------------------------------------------------
void
Arduino::ShutDown()
  {
  if (Serial)
    Serial.println("Arduino::ShutDown()");
  this->state = ProcessorState::SHUTTINGDOWN;
  }

//------------------------------------------------------------------------------
Switch::Switch()
  {
  pinMode(K4, OUTPUT);
  this->PowerOff();
  }

//------------------------------------------------------------------------------
Switch::~Switch()
  {
  this->PowerOff();
  }

//------------------------------------------------------------------------------
void
Switch::Tick64()
  {
  Processor::Tick64();
  }

//------------------------------------------------------------------------------
void
Switch::Tick1024()
  {
  Processor::Tick1024();

  switch(this->state)
    {

    case ProcessorState::POWER_OFF:
      if ((::panel_state == PanelState::DAY) &&
          (::bat_v > BAT_SETPOINT_6)         &&
          (::flarm_allow == true))
        this->PowerOn();
      break;
    case ProcessorState::RUNNING:
      if (::panel_state == PanelState::NIGHT)
        this->ShutDown();
      else
        if (::bat_v < BAT_SETPOINT_4)
          this->ShutDown();
      break;
    default:
      break;
    }
  }

//------------------------------------------------------------------------------
void
Switch::PowerOff()
  {
  if (Serial)
    Serial.println("Switch::PowerOff()");
  digitalWrite(K4, OFF);
  this->state = ProcessorState::POWER_OFF;
  }

//------------------------------------------------------------------------------
void
Switch::PowerOn()
  {
  if (Serial)
    Serial.println("Switch::PowerOn()");
  digitalWrite(K4, ON);
  this->state = ProcessorState::RUNNING;
  }

//------------------------------------------------------------------------------
void
Switch::ShutDown()
  {
  if (Serial)
    Serial.println("Switch::ShutDown()");
  this->state = ProcessorState::SHUTTINGDOWN;
  }
