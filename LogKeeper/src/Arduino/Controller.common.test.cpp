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

#include <WSWire.h>

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
#define PINCLK 9
#define PWM1   11
#define PWM2   10
#define LED    13

ProcessorState ps = ProcessorState::DOWN;
BatteryState   bs = BatteryState::DEAD_FLAT;
PanelState     cs = PanelState::NIGHT;

void i2creceive(int n);
void setBattery();
void setPanel();
bool poll(uint8_t addr, Reg0Response r0, Reg1Response r1);
Reg0Response processorState2Reg0Response(ProcessorState s);
extern void setI2C();
extern uint8_t i2cAddr();
TwoWire WSWire;

//------------------------------------------------------------------------------
void
setup()
  {
  pinMode(PIN00,  INPUT_PULLUP);
  pinMode(PIN01,  INPUT_PULLUP);
  pinMode(PIN04,  INPUT_PULLUP);
  pinMode(PIN05,  INPUT_PULLUP);
  pinMode(PIN06,  INPUT_PULLUP);
  pinMode(PIN07,  INPUT_PULLUP);
  pinMode(PIN08,  INPUT_PULLUP);
  pinMode(PINCLK, INPUT_PULLUP);
  Serial.begin(uint32_t(115200));// For debug.
  setBattery();
  setPanel();
  setI2C();
  // activate internal pullups for twi. Sort of simulate the Pi.
  digitalWrite(SDA, 1);
  digitalWrite(SCL, 1);
  digitalWrite(LED, LOW);
  WSWire.onReceive(i2creceive);
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
                         digitalRead(PIN04) * 4);
    bsc = BatteryState(digitalRead(PIN05) * 1 +
                       digitalRead(PIN06) * 2 +
                       digitalRead(PIN07) * 4);
    csc = PanelState(digitalRead(PIN08) * 1);
    if (bsc != bs || csc != cs)
      {
      bs = bsc;
      cs = csc;
      setBattery();
      setPanel();
      }
    ps = psc;
    }
  poll(i2cAddr(), processorState2Reg0Response(ps), Reg1Response::ENABLEFLARM);
  delay(1000);
  }

//------------------------------------------------------------------------------
void
i2creceive(int n)
  {
  digitalWrite(LED, HIGH);
  if (Serial)
    {
    Serial.print("i2creceive(");
    Serial.print(n, DEC);
    Serial.println(")");
    }
  if (WSWire.available())
    if (WSWire.read() == 0);  // Should be register 0;
      ;
  digitalWrite(LED, LOW);
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

//------------------------------------------------------------------------------
bool
poll(uint8_t addr, Reg0Response r0, Reg1Response r1)
  {
  digitalWrite(LED, HIGH);
  if (Serial)
    {
    Serial.print("poll(");
    Serial.print(byte(r0), DEC);
    Serial.println(")");
    }
  if (r0 != Reg0Response::UNKNOWN)
    {
    WSWire.beginTransmission(32);
    WSWire.write(addr);
    WSWire.write(byte(r0));
    WSWire.write(byte(r1));
    WSWire.endTransmission();
    }
  digitalWrite(LED, LOW);
  return true;
  }

//------------------------------------------------------------------------------
Reg0Response
processorState2Reg0Response(ProcessorState s)
  {
  switch (s)
    {
    case ProcessorState::DOWN:
    case ProcessorState::POWER_ON:
    case ProcessorState::POWER_OFF:
      return Reg0Response::UNKNOWN;
    case ProcessorState::SHUTTINGDOWN:
      return Reg0Response::SHUTTINGDOWN;
    case ProcessorState::BOOTING:
      return Reg0Response::BOOTING;
    case ProcessorState::IDLE:
      return Reg0Response::IDLE;
    case ProcessorState::UP:
      return Reg0Response::RUNNING;
    }
  return Reg0Response::UNKNOWN;
  }
