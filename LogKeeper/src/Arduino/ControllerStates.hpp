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

#ifndef _CONTROLLERSTATES_HPP_
#define _CONTROLLERSTATES_HPP_

enum class BatteryState : byte
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

enum class PanelState : byte
  {
  DAY,
  NIGHT
  };

enum class ProcessorState : byte
  {
  DOWN,
  SHUTTINGDOWN,
  BOOTING,
  IDLE,
  UP,
  POWER_ON,
  POWER_OFF
  };

enum class Reg0Response : byte
  {
  UNKNOWN,
  BOOTING,
  RUNNING,
  IDLE,
  SHUTTINGDOWN,
  };

enum class Reg1Command : byte
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

#endif  // _CONTROLLERSTATES_HPP_
