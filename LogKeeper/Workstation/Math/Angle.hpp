/*
Copyright_License {

  XCSoar Glide Computer - http://www.xcsoar.org/
  Copyright (C) 2000-2015 The XCSoar Project
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
#ifndef ANGLE_HPP
#define ANGLE_HPP

#include "Compiler.h"

#include <type_traits>
#include <utility>
#include <math.h>

#define RAD_TO_DEG (180 / M_PI)
#define DEG_TO_RAD (M_PI / 180)

class Angle
  {
private:
  double value;

  constexpr
  explicit Angle(const double value)
    : value(value)
    {
    };

public:
  /**
   * The default constructor does not initialize the value.  It must
   * not be used until it is assigned.
   */
  Angle() = default;

  constexpr
  static Angle Zero()
    {
    return Native(double(0));
    }

  constexpr
  static Angle Native(const double value)
    {
    return Angle(value);
    }

  /**
   * Construct an instance that describes a "full circle" (360
   * degrees).
   */
  constexpr
  static Angle FullCircle()
    {
#ifdef RADIANS
    return Native(double_two_pi);
#else
    return Native(double(360));
#endif
    }

  /**
   * Construct an instance that describes a "half circle" (180
   * degrees).
   */
  constexpr
  static Angle HalfCircle()
    {
#ifdef RADIANS
    return Native(double_pi);
#else
    return Native(double(180));
#endif
    }

  /**
   * Construct an instance that describes a "quarter circle" 90
   * degrees).
   */
  constexpr
  static Angle QuarterCircle()
    {
#ifdef RADIANS
    return Native(double_half_pi);
#else
    return Native(double(90));
#endif
    }

  constexpr
  double Native() const
    {
    return value;
    }

#ifdef RADIANS
  constexpr
  static Angle Degrees(int value)
    {
    return Angle(double(value * DEG_TO_RAD));
    }

  constexpr
  static Angle Degrees(unsigned value)
    {
    return Degrees(int(value));
    }

  constexpr
  static Angle Degrees(double value) 
    {
    return Angle(double(value * DEG_TO_RAD));
    }

  constexpr
  static Angle Radians(const double value)
    {
    return Angle(value);
    }

  constexpr
  double Degrees() const
    {
    return this->value * RAD_TO_DEG;
    }

  constexpr
  double Radians() const
    {
    return value;
    }

  constexpr
  double Hours() const
    {
    return fast_mult(value, double(24 / M_2PI), 4);
    }
#else
  constexpr
  static Angle Degrees(const double value) 
    {
    return Angle(value);
    }

  constexpr
  static Angle Degrees(int value) 
    {
    return Angle(double(value));
    }

  constexpr
  static Angle Degrees(unsigned value)
    {
    return Degrees(int(value));
    }

  constexpr
  static Angle Radians(double value)
    {
    return Angle(double(value * RAD_TO_DEG));
    }

  constexpr
  double Degrees() const
    {
    return this->value;
    }

  constexpr
  double Radians() const
    {
    return this->value * DEG_TO_RAD;
    }

  gcc_pure
  double Hours() const
    {
    return this->value * double(24. / 360.);
    }
#endif

  constexpr
  static Angle DMS(unsigned d, unsigned m, unsigned s)
    {
    return Angle::Degrees(double(d) + double(m) / 60 + double(s) / 3600);
    }

  /**
   * Converts this Angle to degrees, minute, seconds and a
   * bool-based east/north variable
   *
   * @param dd Degrees (pointer)
   * @param mm Minutes (pointer)
   * @param ss Seconds (pointer)
   * @param east True if East, False if West (pointer)
   */
  void ToDMS(unsigned &dd, unsigned &mm, unsigned &ss,
             bool &is_positive) const;

  /**
   * Converts this Angle to degrees, minute, decimal minutes and a
   * bool-based east/north variable
   *
   * @param dd Degrees (pointer)
   * @param mm Minutes (pointer)
   * @param mmm Decimal minutes (pointer)
   * @param east True if East, False if West (pointer)
   */
  void ToDMM(unsigned &dd, unsigned &mm, unsigned &mmm,
             bool &is_positive) const;

  gcc_pure
  Angle Absolute() const
    {
    return Angle(fabs(Native()));
    }

  /**
   * Calculates the tangent of the Angle.
   */
  gcc_pure
  inline double tan() const 
    {
    return ::tan(Radians());
    }

  /**
   * Calculates the sine of the Angle.
   */
  gcc_pure
  inline double sin() const
    {
    return ::sin(Radians());
    }

  gcc_pure
  inline double accurate_half_sin() const
    {
    return ::sin(Radians() / 2.0);
    }

  /**
   * Calculates the cosine of the Angle.
   */
  gcc_pure
  inline double cos() const
    {
    return ::cos(Radians());
    }

  gcc_pure
  inline double invfastcosine() const
    {
    return (::acos(Native()));
    }

  /**
   * Returns the sine of the Angle as an integer
   * in the range between -1024 and 1024.
   */
  gcc_pure
  inline int ifastsine() const
    {
    return (::asin(Native()) * 1024);
    }

  /**
   * Returns the cosine of the Angle as an integer
   * in the range between -1024 and 1024.
   */
  gcc_pure
  inline int ifastcosine() const
    {
    return (::cos(Native()) * 1024);
    }

  gcc_pure
  int Sign() const;

  gcc_pure
  int Sign(const double tolerance) const;

  gcc_pure
  std::pair<double, double> SinCos() const 
    {
    std::pair<double, double> rtn;
    rtn.first  = ::sin(this->Radians());
    rtn.second = ::cos(this->Radians());
    return rtn;
    }

  gcc_pure
  double AbsoluteDegrees() const;

  gcc_pure
  double AbsoluteRadians() const;

  void Flip() 
    {
    this->value = -this->value;
    }

  constexpr
  Angle Flipped() const
    {
    return Angle(-this->value);
    }

  /**
   * Limits the angle (theta) to -180 - +180 degrees
   * @return Output angle (-180 - +180 degrees)
   */
  gcc_pure
  Angle AsDelta() const;

  /**
   * Limits the angle (theta) to 0 - 360 degrees
   * @return Output angle (0-360 degrees)
   */
  gcc_pure
  Angle AsBearing() const;

  /**
   * Returns half of this angle.  This is only useful (and valid) when
   * the angle has been normalized with AsDelta().
   */
  constexpr
  Angle Half() const
    {
    return Angle(this->value / 2.0);
    }

  /**
   * Rotate angle by 180 degrees and limit to 0 - 360 degrees
   * @return Output angle (0 - 360 degrees)
   */
  gcc_pure
  Angle Reciprocal() const;

  gcc_pure
  Angle HalfAngle(const Angle end) const;

  /**
   * Computes a certain fraction between the two angles.
   *
   * @param fraction a fraction between 0 and 1
   * @return the resulting Angle, not normalized
   */
  gcc_pure
  Angle Fraction(const Angle end, const double fraction) const;

  constexpr Angle
  operator*(const Angle x) const
    {
    return Angle(this->value * x.value);
    }

  gcc_pure Angle
  operator*(const double x) const
    {
    return Angle(this->value * x);
    }

  constexpr
  Angle
  operator*(const int x) const
    {
    return Angle(this->value * x);
    }

  constexpr
  Angle
  operator*(const unsigned x) const
    {
    return Angle(this->value * x);
    }

  gcc_pure
  Angle
  operator/(const double x) const
    {
    return Angle(this->value / x);
    }

  gcc_pure
  double
  operator/(const Angle x) const
    {
    return this->value / x.value;
    }

  constexpr
  Angle
  operator/(const int x) const
    {
    return Angle(this->value / x);
    }

  constexpr
  Angle
  operator/(const unsigned x) const
    {
    return Angle(this->value / x);
    }

  constexpr
  Angle
  operator+(const Angle x) const
    {
    return Angle(this->value + x.value);
    }

  constexpr
  Angle
  operator-(const Angle x) const
    {
    return Angle(this->value - x.value);
    }

  constexpr
  Angle
  operator-() const
    {
    return Angle(-this->value);
    }

  const Angle&
  operator*=(const double x)
    {
    this->value *= x;
    return *this;
    }

  const Angle&
  operator+=(Angle x)
    {
    this->value += x.value;
    return *this;
    }

  const Angle&
  operator-=(Angle x)
    {
    this->value -= x.value;
    return *this;
    }

  constexpr bool
  operator==(const Angle x) const
    {
    return this->value == x.value;
    }

  constexpr bool
  operator!=(const Angle x) const
    {
    return this->value != x.value;
    }

  constexpr bool
  operator<(const Angle x) const
    {
    return this->value < x.value;
    }

  constexpr bool
  operator>(const Angle x) const
    {
    return this->value > x.value;
    }

  constexpr bool
  operator<=(const Angle x) const
    {
    return this->value <= x.value;
    }

  constexpr bool
  operator>=(const Angle x) const
    {
    return this->value >= x.value;
    }

  /**
   * Is this angle between the other two values?  If "end" is smaller
   * than "start", then wraparound is calculated correctly.
   */
  gcc_pure
  bool Between(const Angle start, const Angle end) const;

  gcc_const
  static Angle asin(double x)
    {
    return Radians(::asin(x));
    }

  gcc_const
  static Angle acos(double x) 
    {
    return Radians(::acos(x));
    }

  /**
   * Returns the angle based on the input of both a x- and y-coordinate.
   * This is the mathematical angle where zero means along x axis and
   * the positive direction is counter-clockwise!
   * @param x x-coordinate
   * @param y y-coordinate
   * @return Counter-clockwise angle between the x-axis and the given coordinate
   */
  gcc_const
  static Angle FromXY(const double x, const double y)
    {
    return Angle::Radians(atan2(y,x));
    }

  /**
   * Check whether the two angles are roughly equal.
   */
  gcc_const
  bool CompareRoughly(Angle other, Angle threshold = Angle::Degrees(10)) const;

  gcc_pure
  bool IsPositive() const
    {
    return this->value > 0;
    }

  gcc_pure
  bool IsNegative() const 
    {
    return std::signbit(this->value);
    }

  };

static_assert(std::is_trivial<Angle>::value, "type is not trivial");

#endif  // ANGLE_HPP
