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

#include "DeltaTime.hpp"
#include <assert.h>

//------------------------------------------------------------------------------
double
DeltaTime::Update(double current_time, double min_delta, double warp_tolerance)
  {
  assert(current_time >= 0.0);
  assert(min_delta >= 0.0);
  assert(warp_tolerance >= 0.0);

  if (!this->IsDefined())
    {
    /* first call */
    this->last_time = current_time;
    return double(0);
    }

  if (current_time < this->last_time)
    {
    /* time warp */

    const double delta = this->last_time - current_time;
    this->last_time = current_time;
    return delta < warp_tolerance ? double(0) : double(-1);
    }

  const double delta = current_time - this->last_time;
  if (delta < min_delta)
    /* difference too small, don't update "last" time stamp to let
       small differences add up eventually */
    return double(0);

  this->last_time = current_time;

  if (delta > double(4 * 3600))
    /* after several hours without a signal, we can assume there was
       a time warp */
    return double(-1);

  return delta;
  }
