#ifndef ND_MATH_UNITS_ANGLE_HPP
#define ND_MATH_UNITS_ANGLE_HPP

#include <cmath>
#include <ostream>

#include "quantity.hpp"
#include "ratio.hpp"
#include "traits.hpp"

namespace nd::math::units
{

template <typename ValueType, typename AngleRatio>
using Angle			= Quantity<ValueType, Dimension<>, Dimension<>, Dimension<>, Dimension<>, Dimension<>, Dimension<>, Dimension<>, AngleRatio>;

template <typename ValueType>
using Radians		= Angle<ValueType, Ratio<>>;

template <typename ValueType>
using Degrees		= Angle<ValueType, Ratio<18'446'744'073'709'551'600u, 321'956'420'358'983'237u>>; // (180 / pi) approximated for 64 bit unsigned integer

template <typename ValueType>
using Revolutions	= Angle<ValueType, Ratio<2'935'890'503'282'001'226u, 18'446'744'073'709'551'612u>>; // (1 / 2pi) approximated for 64 bit unsigned integer

template <typename ValueType>
using Gons			= Angle<ValueType, Ratio<18'446'744'073'709'551'600u, 289'760'778'323'084'913u>>; // (200 / pi) approximated for 64 bit unsigned integer

template <typename ValueType>
std::ostream &operator<<(std::ostream &stream, const Radians<ValueType> &angle)
{
	stream << std::to_string(angle.radians()) << " rad";
	return stream;
}

namespace constants
{

template <typename ValueType>
inline constexpr Radians<ValueType> pi	= static_cast<ValueType>(M_PI);

template <typename ValueType>
inline constexpr Radians<ValueType> tau	= static_cast<ValueType>(2) * pi<ValueType>;

} // namespace nd::math::units::constants

namespace literals
{

constexpr Radians<double> operator""_rad(const long double radians)
{
	return radians;
}

constexpr Degrees<double> operator""_deg(const long double degrees)
{
	return degrees;
}

constexpr Revolutions<double> operator""_r(const long double revolutions)
{
	return revolutions;
}

constexpr Gons<double> operator""_gon(const long double gons)
{
	return gons;
}

} // namespace nd::math::units::literals

} // namespace nd::math::units

#endif // ND_MATH_UNITS_ANGLE_HPP
