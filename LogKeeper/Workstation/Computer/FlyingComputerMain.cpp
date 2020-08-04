/*
Copyright_License {

  Glider Operations.
  Copyright (C) 2013-2020 Peter F Bradshaw
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

#include "FlyingComputer.hpp"
#include "FlyingState.hpp"
#include "Info.hpp"

#include <string>
#include <array>
#include <istream>
#include <iostream>
#include <sstream>
#include <math.h>
#include <stdlib.h>

enum  // Variable indices in the array.
  {
  TIME,
  LAT,
  LON,
  ALT,
  GS,
  VS,
  END_ARRAY
  };
const double field_height = 310;

//------------------------------------------------------------------------------
int
main()
  {
  FlyingState state;
  state.Reset();
  FlyingComputer computer;
  computer.Reset();
  Info basic;
  basic.alive.Clear();
  computer.FieldAltitude(310);

  while (!std::cin.eof())
    {
    std::string sin;
    std::array<std::string, END_ARRAY> var;

    std::getline(std::cin, sin);
    std::stringstream streamin;
    streamin.str(sin);
    std::string token;
    bool res;
    unsigned int i;
    for (res = false, i = 0; (res == true) && (i < END_ARRAY); i++)
      {
      if (std::getline(streamin, token, ' '))
        res = true;
      else
        {
        res = false;
        break;
        }
      var[i] = token;
      }
    Angle alat = Angle::Native(stof(var[LAT]));
    Angle alon = Angle::Native(stof(var[LON]));
    GeoPoint loc(alat * DEG_TO_RAD, alon * DEG_TO_RAD);
    basic.SetFileLocation(std::stof(var[TIME]),
                          loc,
                          std::stof(var[ALT]),
                          std::stof(var[GS]),
                          std::stof(var[VS]));

    /*
     * Compute Info.
     */
    computer.Compute(16.6667, // 60 km / hr
                     basic,
                     state);
                     
    }
  std::cout << state.takeoff_time << ", "
            << state.landing_time << std::endl;
  return 0;
  }
