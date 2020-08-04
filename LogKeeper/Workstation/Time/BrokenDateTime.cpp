/*
Copyright_License {

  XCSoar Glide Computer - http://www.xcsoar.org/
  Copyright (C) 2000-2015 The XCSoar Project
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

#include "BrokenDateTime.hpp"
#include "DateUtil.hpp"

#include <assert.h>
#include <time.h>

//------------------------------------------------------------------------------
static const BrokenDateTime
ToBrokenDateTime(const struct tm &tm)
  {
  BrokenDateTime dt;

  dt.year = tm.tm_year + 1900;
  dt.month = tm.tm_mon + 1;
  dt.day = tm.tm_mday;
  dt.day_of_week = tm.tm_wday;

  dt.hour = tm.tm_hour;
  dt.minute = tm.tm_min;
  dt.second = tm.tm_sec;

  return dt;
  }

//------------------------------------------------------------------------------
BrokenDateTime
BrokenDateTime::FromUnixTimeUTC(int64_t _t)
  {
  time_t t = (time_t)_t;
  struct tm tm;
  gmtime_r(&t, &tm);

  return ToBrokenDateTime(tm);
  }

//------------------------------------------------------------------------------
int64_t
BrokenDateTime::ToUnixTimeUTC() const
  {
  assert(IsPlausible());

  struct tm tm;
  tm.tm_year = year - 1900;
  tm.tm_mon = month - 1;
  tm.tm_mday = day;
  tm.tm_hour = hour;
  tm.tm_min = minute;
  tm.tm_sec = second;
  tm.tm_isdst = 0;
  return ::timegm(&tm);
  }

//------------------------------------------------------------------------------
const BrokenDateTime
BrokenDateTime::NowUTC()
  {
  time_t t = time(NULL);
  return FromUnixTimeUTC(t);
  }

//------------------------------------------------------------------------------
const BrokenDateTime
BrokenDateTime::NowLocal()
  {
  time_t t = time(NULL);
  struct tm tm;
  localtime_r(&t, &tm);

  return ToBrokenDateTime(tm);
  }

//------------------------------------------------------------------------------
BrokenDateTime
BrokenDateTime::operator+(int seconds) const
  {
  assert(IsPlausible());

  return FromUnixTimeUTC(ToUnixTimeUTC() + seconds);
  }

//------------------------------------------------------------------------------
int
BrokenDateTime::operator-(const BrokenDateTime &other) const
  {
  return ToUnixTimeUTC() - other.ToUnixTimeUTC();
  }
