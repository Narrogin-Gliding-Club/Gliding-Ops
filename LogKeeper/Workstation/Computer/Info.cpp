/*
Copyright_License {

  XCSoar Glide Computer - http://www.xcsoar.org/
  Copyright (C) 2000-2020 The XCSoar Project
  Copyright (C) 2000-2020 Peter F Bradshaw
  Copyright (C) 2000-2020 Narrogin Gliding Club
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

#include "Info.hpp"

//------------------------------------------------------------------------------
void
Info::UpdateClock()
  {
//  clock = double(MonotonicClockMS()) / 1000;  // Set clock from file
  }

//------------------------------------------------------------------------------
BrokenDateTime
Info::GetDateTimeAt(double other_time) const
  {
  if (other_time < 0.0)
    return BrokenDateTime::Invalid();

  if (!time_available || !date_time_utc.IsDatePlausible())
    return BrokenDateTime(BrokenDate::Invalid(),
                          BrokenTime::FromSecondOfDayChecked(int(other_time)));

  return date_time_utc + int(other_time - time);
  }

//------------------------------------------------------------------------------
void
Info::ProvideTime(double time)
  {
  assert(time >= 0.0);

  this->time = time;
  this->time_available.Update(clock);

  unsigned t = (unsigned)time;
  this->date_time_utc.second = t % 60;
  t /= 60;

  this->date_time_utc.minute = t % 60;
  t /= 60;

  this->date_time_utc.hour = t % 24;
  }

//------------------------------------------------------------------------------
void
Info::ProvideDate(const BrokenDate &date)
  {
  assert(date.IsPlausible());

  (BrokenDate &)this->date_time_utc = date;
  }

//------------------------------------------------------------------------------
void
Info::ProvideTrueAirspeedWithAltitude(double tas, double altitude)
  {
  this->true_airspeed = tas;
  this->indicated_airspeed = true_airspeed;
  this->airspeed_available.Update(clock);
  this->airspeed_real = true;
  }

//------------------------------------------------------------------------------
void
Info::ProvideIndicatedAirspeedWithAltitude(double ias, double altitude)
  {
  this->indicated_airspeed = ias;
  this->true_airspeed = indicated_airspeed;
  this->airspeed_available.Update(clock);
  this->airspeed_real = true;
  }

//------------------------------------------------------------------------------
void
Info::ProvideTrueAirspeed(double tas)
  {
  auto any_altitude = GetAnyAltitude();

  if (any_altitude.first)
    this->ProvideTrueAirspeedWithAltitude(tas, any_altitude.second);
  else
    /* no altitude; dirty fallback */
    this->ProvideBothAirspeeds(tas, tas);
  }

//------------------------------------------------------------------------------
void
Info::ProvideIndicatedAirspeed(double ias)
  {
  auto any_altitude = GetAnyAltitude();

  if (any_altitude.first)
    this->ProvideIndicatedAirspeedWithAltitude(ias, any_altitude.second);
  else
    /* no altitude; dirty fallback */
    this->ProvideBothAirspeeds(ias, ias);
  }

//------------------------------------------------------------------------------
void
Info::Reset()
  {
  this->UpdateClock();

  this->alive.Clear();

  this->location_available.Clear();

  this->track = Angle::Zero();
  this->track_available.Clear();

  this->heading_available.Clear();

  this->variation_available.Clear();

  this->ground_speed_available.Clear();
  this->airspeed_available.Clear();
  this->ground_speed = this->true_airspeed = this->indicated_airspeed = double(0);
  this->airspeed_real = false;

  this->gps_altitude_available.Clear();

  this->sensor_calibration_available.Clear();

  this->baro_altitude_available.Clear();
  this->baro_altitude = double(0);

  this->time_available.Clear();
  this->time = double(0);

  this->date_time_utc = BrokenDateTime::Invalid();

  this->noncomp_vario_available.Clear();
  this->total_energy_vario_available.Clear();
  this->netto_vario_available.Clear();

  this->external_wind_available.Clear();

  this->temperature_available = false;
  this->humidity_available = false;

  this->engine_noise_level_available.Clear();

  this->voltage_available.Clear();
  this->battery_level_available.Clear();

  this->stall_ratio_available.Clear();
  }

//------------------------------------------------------------------------------
void
Info::ExpireWallClock()
  {
  if (!this->alive)
    return;

  this->UpdateClock();

  this->alive.Expire(clock, double(10));
  if (!this->alive)
    {
#if 0
    time_available.Clear();
    gps.Reset();
    flarm.Clear();
#endif
    ;
    }
  else
    {
    this->time_available.Expire(clock, double(10));
    }
  }

//------------------------------------------------------------------------------
void
Info::Expire()
  {
  this->location_available.Expire(clock, double(10));
  this->track_available.Expire(clock, double(10));
  this->ground_speed_available.Expire(clock, double(10));

  if (this->airspeed_available.Expire(clock, double(30)))
    this->airspeed_real = false;

  this->gps_altitude_available.Expire(clock, double(30));
  this->sensor_calibration_available.Expire(clock, double(3600));
  this->baro_altitude_available.Expire(clock, double(30));
  this->noncomp_vario_available.Expire(clock, double(5));
  this->total_energy_vario_available.Expire(clock, double(5));
  this->netto_vario_available.Expire(clock, double(5));
  this->external_wind_available.Expire(clock, double(600));
  this->engine_noise_level_available.Expire(clock, double(30));
  this->voltage_available.Expire(clock, double(300));
  this->battery_level_available.Expire(clock, double(300));
  }

//------------------------------------------------------------------------------
void
Info::Complement(const Info &add)
  {
  if (!add.alive)
    /* if there is no heartbeat on the other object, there cannot be
       useful information */
    return;

  this->alive.Complement(add.alive);

  if (this->time_available.Complement(add.time_available))
    {
    this->time = add.time;
    this->date_time_utc = add.date_time_utc;
    }

  if (this->location_available.Complement(add.location_available))
    {
    this->location = add.location;
    }

  if (this->track_available.Complement(add.track_available))
    this->track = add.track;

  if (this->ground_speed_available.Complement(add.ground_speed_available))
    this->ground_speed = add.ground_speed;

  if ((add.airspeed_real || !this->airspeed_real) &&
      this->airspeed_available.Complement(add.airspeed_available))
    {
    this->true_airspeed = add.true_airspeed;
    this->indicated_airspeed = add.indicated_airspeed;
    this->airspeed_real = add.airspeed_real;
    }

  if (this->gps_altitude_available.Complement(add.gps_altitude_available))
    this->gps_altitude = add.gps_altitude;

  if (this->sensor_calibration_available.Complement(add.sensor_calibration_available))
    {
    this->sensor_calibration_factor = add.sensor_calibration_factor;
    this->sensor_calibration_offset = add.sensor_calibration_offset;
    }

  if (this->baro_altitude_available.Complement(add.baro_altitude_available))
    this->baro_altitude = add.baro_altitude;

  if (this->noncomp_vario_available.Complement(add.noncomp_vario_available))
    this->noncomp_vario = add.noncomp_vario;

  if (this->total_energy_vario_available.Complement(add.total_energy_vario_available))
    this->total_energy_vario = add.total_energy_vario;

  if (this->netto_vario_available.Complement(add.netto_vario_available))
    this->netto_vario = add.netto_vario;

  if (this->external_wind_available.Complement(add.external_wind_available))
    this->external_wind = add.external_wind;

  if (!this->temperature_available && add.temperature_available)
    {
    this->temperature = add.temperature;
    this->temperature_available = add.temperature_available;
    }

  if (!this->heading_available && add.heading_available)
    {
    this->heading = add.heading;
    this->heading_available = add.heading_available;
    }

  if (!this->variation_available && add.variation_available)
    {
    this->variation = add.variation;
    this->variation_available = add.variation_available;
    }

  if (!this->humidity_available && add.humidity_available)
    {
    this->humidity = add.humidity;
    this->humidity_available = add.humidity_available;
    }

  if (this->voltage_available.Complement(add.voltage_available))
    this->voltage = add.voltage;

  if (this->battery_level_available.Complement(add.battery_level_available))
    this->battery_level = add.battery_level;

  if (!this->stall_ratio_available && add.stall_ratio_available)
    this->stall_ratio = add.stall_ratio;
  }
