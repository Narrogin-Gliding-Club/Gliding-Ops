/*
Copyright_License {

  XCSoar Glide Computer - http://www.xcsoar.org/
  Copyright (C) 2000-2016 The XCSoar Project
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

#ifndef STATE_CLOCK_HPP
#define STATE_CLOCK_HPP

#include <algorithm>

#include <cassert>

/**
 * Track how much time the aircraft was in a certain state
 * (e.g. "moving").  It provides tools to decrement the measured
 * duration while the aircraft was not in that state.
 *
 * @param max_value the maximum value; at this peak, the countdown
 * will start, and after this duration, the state will be cleared
 * @param max_delta the maximum delta accepted by Add() and
 * Subtract(), to avoid hiccups due to temporary GPS outage
 */
template<unsigned max_value, unsigned max_delta>
class StateClock
  {
    double value;

  public:
    void Clear()
      {
      this->value = 0;
      }

    bool IsDefined() const
      {
      assert(this->value >= 0);

      return this->value > 0;
      }

  private:
    static double LimitDelta(double delta)
      {
      assert(delta >= 0);

      return std::min(delta, double(max_delta));
      }

  public:
    void Add(double delta)
      {
      assert(this->value >= 0);
      assert(this->value <= double(max_value));

      this->value += LimitDelta(delta);
      if (this->value > double(max_value))
        this->value = double(max_value);
      }

    void Subtract(double delta)
      {
      assert(this->value >= 0);
      assert(this->value <= double(max_value));

      this->value -= LimitDelta(delta);
      if (this->value < 0)
        this->value = 0;
      }

    bool operator>=(double other) const
      {
      assert(other > 0);
      assert(this->value >= 0);
      assert(this->value <= double(max_value));

      return this->value >= other;
      }
  };

#endif  // STATE_CLOCK_HPP
