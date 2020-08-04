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

#ifndef FLYING_COMPUTER_HPP
#define FLYING_COMPUTER_HPP

#include "StateClock.hpp"
#include "Geo/GeoPoint.hpp"
#include "Math/Angle.hpp"
#include "Time/DeltaTime.hpp"

struct Info;
struct DerivedInfo;
struct AircraftState;
struct FlyingState;

/**
 * Detect takeoff and landing.
 */
class FlyingComputer
  {
  DeltaTime delta_time;

  /**
   * Tracks the duration the aircraft has been stationary.
   */
  StateClock<60, 5> stationary_clock;

  /**
   * Tracks the duration the aircraft has been moving.
   */
  StateClock<30, 5> moving_clock;

  /**
   * Tracks the duration the aircraft has been climbing.  If the
   * aircraft has been climbing for a certain amount of time, it is
   * assumed that it is still flying, even if the ground speed is
   * small (for example, when flying in a wave without airspeed
   * input).
   */
  StateClock<20, 5> climbing_clock;

  /**
   * If the aircraft is currently assumed to be moving, then this
   * denotes the initial moving time stamp.  This gets reset to a
   * negative value when the aircraft is stationary for a certain
   * amount of time.
   */
  double moving_since;

  /**
   * If the aircraft is currently assumed to be moving, then this
   * denotes the location when moving started initially.  This
   * attribute is only valid if #moving_since is non-negative.
   */
  GeoPoint moving_at;

  double stationary_since;
  GeoPoint stationary_at;

  /**
   * If the aircraft is in powered flight, then this denotes
   * the initial powered time stamp. If the aircraft is unpowered
   * this is set to a negative value.
   */
  double powered_since;
  GeoPoint powered_at;

  /**
   * If the aircraft is in unpowered flight, then this denotes
   * the initial unpowered time stamp. If the aircraft is powered
   * this is set to a negative value.
   */
  double unpowered_since;
  GeoPoint unpowered_at;

  double climbing_altitude;

  double sinking_since;

  GeoPoint sinking_location;

  double sinking_altitude;

  /**
   * The last altitude when the aircraft was supposed to be on the
   * ground.  This is usually the elevation of the take-off airfield.
   *
   * A negative value means unknown.  Sorry for these few airfields
   * that are below MSL ...
   */
  double last_ground_altitude;

  double field_altitude;

public:
  void Reset();

  /**
   * This will compute the current state of the aircraft.
   * @param takeoff_speed A reasonable takeoff speed for this aircraft.
   * @param basic The current input state.
   * @param flying The current aircraft state as output.
   */
  void Compute(double takeoff_speed,
               const Info &basic,
               FlyingState &flying);

  void Compute(double takeoff_speed,
               const AircraftState &state,
               double dt,
               FlyingState &flying);

  /**
   * Finish the landing detection.  If landing has not been detected,
   * but the aircraft has not been moving, this force-detects the
   * landing now.  Call at the end of a replay.
   */
  void Finish(FlyingState &flying, double time);

  /**
   * Set the altitude of this field
   * alitude Altitude in meters.
   */
  void FieldAltitude(double altitude);

 /**
  * Get the current field altitude.
  * @return The FieldAltitude in meters.
  */
  double FieldAltitude() const;

protected:
  void CheckRelease(FlyingState &state,
                    double time,
                    const GeoPoint &location,
                    double altitude);

  /**
   * Check for monotonic climb.  This check is used for "flying"
   * detection in a wave, when ground speed is low, no airspeed is
   * available and no map was loaded.
   *
   * @return true if the aircraft has been climbing for more than 10
   * seconds
   */
  bool CheckClimbing(double dt,
                     double altitude);

  /**
   * Check for powered flight.
   */
  void CheckPowered(double dt,
                    const Info &basic,
                    FlyingState &flying);

  void Check(FlyingState &state, double time);

  /**
   * Update flying state when moving
   *
   * @param time Time the aircraft is moving
   */
  void Moving(FlyingState &state,
              double time,
              double dt,
              const GeoPoint &location);

  /**
   * Update flying state when stationary
   *
   * @param state Current aircraft state.
   * @param time Time the aircraft is stationary.
   * @param dt Time since last call.
   * @param location Current location.
   */
  void Stationary(FlyingState &state,
                  double time,
                  double dt,
                  const GeoPoint &location);
  };

#endif  // FLYING_COMPUTER_HPP
