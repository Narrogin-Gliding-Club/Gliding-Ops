/*
 * Glider Operations.
 * Copyright (C) 2013-2017 Peter F Bradshaw
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * - Redistributions of source code must retain the above copyright
 * notice, this list of conditions and the following disclaimer.
 *
 * - Redistributions in binary form must reproduce the above copyright
 * notice, this list of conditions and the following disclaimer in the
 * documentation and/or other materials provided with the
 * distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE
 * FOUNDATION OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED
 * OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef _CONTROLLER_HPP_
#define _CONTROLLER_HPP_

#include <Arduino.h>

enum class BatteryState
  {
  DEAD_FLAT,
  FLAT,
  DISCHARGED,
  REDUCED,
  PARTIAL,
  NOMINAL,
  FULL,
  OVER,
  };

enum class PanelState
  {
  DAY,
  NIGHT
  };

enum class ProcessorState
  {
  DOWN,
  SHUTTINGDOWN,
  BOOTING,
  IDLE,
  RUNNING,
  POWER_ON,
  POWER_OFF
  };

enum class Reg0Response
  {
  UNKNOWN,
  BOOTING,
  RUNNING,
  IDLE,
  SHUTTINGDOWN,
  };

enum class Reg1Response
  {
  NONE,
  SHUTDOWNFLARM,
  ENABLEFLARM,    // Signal that FLARM may be turned on today.
  SHUTDOWNADSL,
  };

enum class Command
  {
  NONE,
  DOWN,
  KILL_APP,
  LAUNCH_APP,
  };

class Processor
  {
public:
  Processor();

  virtual ~Processor();

  virtual void KillApp();
  virtual void RunApp();
  virtual void Tick2();
  virtual void Tick4();
  virtual void Tick8();
  virtual void Tick32();
  virtual void Tick64();
  virtual void Tick1024();
  ProcessorState State() const;

protected:
  virtual void PowerOff() = 0;
  virtual void PowerOn() = 0;
  virtual void ShutDown() = 0;

  ProcessorState state;
  uint8_t power_off_timer;

private:
  };

class Adsl : public Processor
  {
public:
  Adsl();

  virtual ~Adsl();

  void KillApp();
  void RunApp();
  void Tick1024();

protected:
  void PowerOff();
  void PowerOn();
  void ShutDown();

private:
  };

class Flarm : public Processor
  {
public:
  Flarm();

  virtual ~Flarm();

  void KillApp();
  void RunApp();
  void Tick1024();

protected:
  void PowerOff();
  void PowerOn();
  void ShutDown();

private:
  };

class Arduino : public Processor
  {
public:
  Arduino();

  virtual ~Arduino();
  void Tick8();
  void Tick32();
  void Tick64();
  void Tick1024();

protected:
  void PowerOff();
  void PowerOn();
  void ShutDown();

private:
  uint8_t led_state;
  };

#endif // _CONTROLLER_HPP_
