/*
Copyright_License {

  XCSoar Glide Computer - http://www.xcsoar.org/
  Copyright (C) 2000-2015 The XCSoar Project
  Copyright (C) 2020-2020 Peter F Bradshaw
  Copyright (C) 2020-2020 Narrogin Gliding Club
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

#include "FlyingState.hpp"

//------------------------------------------------------------------------------
void
FlyingState::Reset()
  {
  this->flying = false;
  this->on_ground = false;
  this->powered = false;
  this->flight_time = double(0);
  this->takeoff_time = double(-1);
  this->takeoff_location.SetInvalid();
  this->release_time = double(-1);
  this->release_location.SetInvalid();
  this->power_on_time = double(-1);
  this->power_on_location.SetInvalid();
  this->power_off_time = double(-1);
  this->power_off_location.SetInvalid();
  this->far_location.SetInvalid();
  this->far_distance = double(-1);
  this->landing_time = double(-1);
  this->landing_location.SetInvalid();
  }
