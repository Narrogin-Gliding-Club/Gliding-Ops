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

#include "Controller.test.hpp"
#include "Arduino.h"

#include "Controller.hpp"

#include <WSWire.h>

#define PIN00  0
#define PIN01  1
#define PIN02  2
#define PIN03  3
#define PIN04  4
#define PIN05  5
#define PIN06  6
#define PIN07  7
#define PIN08  8
#define PIN12 12
#define PIN13 13
#define PINCLK 9
#define PWM1   11
#define PWM2   10
#define LED    13

BatteryState   bs = BatteryState::DEAD_FLAT;
PanelState     cs = PanelState::NIGHT;
bool p0_up  = false;
bool p0_app = false;
bool p1_up  = false;
bool p1_app = false;

uint16_t tick;
uint8_t  flarm_allow;

void setBattery();
void setPanel();
bool poll(uint8_t addr, uint8_t data);
void i2creceive(int n);

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
  pinMode(PIN12,  INPUT_PULLUP);
  pinMode(PIN13,  INPUT_PULLUP);
  pinMode(PINCLK, INPUT_PULLUP);
  Serial.begin(uint32_t(115200));// For debug.
  WSWire.begin(64);
  digitalWrite(SCL, 1);   // Turn on i2c pullups.
  digitalWrite(SDA, 1);
  WSWire.onReceive(i2creceive);
  setBattery();
  setPanel();
  tick = 0;
  flarm_allow = 0;
  }

//------------------------------------------------------------------------------
void
loop()
  {
  if (digitalRead(PINCLK) == LOW)
    {   // Don't bother to debounce.
    BatteryState   bsc;
    PanelState     csc;
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
    }

  if (tick % 10 == 0)
    {
    p0_up  = !bool(digitalRead(PIN00));
    p0_app = !bool(digitalRead(PIN01));
    p1_up  = !bool(digitalRead(PIN12));
    p1_app = !bool(digitalRead(PIN13));
    if (Serial)
      {
      Serial.print("Adsl processor: ");
      Serial.print((p0_up  == true) ? "UP" : "DOWN");
      Serial.print(", Adsl app: ");
      Serial.print((p0_app == true) ? "RUN" : "IDLE");
      Serial.print(", Flarm processor: ");
      Serial.print((p1_up  == true) ? "UP" : "DOWN");
      Serial.print(", Flarm app: ");
      Serial.print((p1_app == true) ? "RUN" : "IDLE");
      Serial.println();
      }
    }
  if (tick % 100 == 0)
    poll(32, flarm_allow);
  if (tick >= 10000)
    {
    if (flarm_allow == 0)
      flarm_allow = 1;
    else
      flarm_allow = 0;
    tick = 0;
    }
  tick++;

  delay(100);
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
poll(uint8_t addr, uint8_t data)
  {
  WSWire.beginTransmission(addr);
  WSWire.write(data);
  return (WSWire.endTransmission() == 0) ? true : false;
  }

//------------------------------------------------------------------------------
void
i2creceive(int n)
  {
  }
