#ifndef ND_MATH_UNITS_METER_HPP
#define ND_MATH_UNITS_METER_HPP

#include "length.hpp"
#include "ratio.hpp"

namespace nd::math::units
{

template <typename ValueType>
using Femtometer	= Length<ValueType, Femto>;

template <typename ValueType>
using Fermi			= Femtometer<ValueType>;

template <typename ValueType>
using Picometer		= Length<ValueType, Pico>;

template <typename ValueType>
using Angstrom		= Length<ValueType, RatioMul<Deci, Nano>>;

template <typename ValueType>
using Nanometer		= Length<ValueType, Nano>;

template <typename ValueType>
using MilliMicron	= Length<ValueType, RatioMul<Milli, Micro>>;

template <typename ValueType>
using Micrometer	= Length<ValueType, Micro>;

template <typename ValueType>
using Micron		= Micrometer<ValueType>;

template <typename ValueType = double>
using Millimeter	= Length<ValueType, Milli>;

template <typename ValueType>
using Centimeter	= Length<ValueType, Centi>;

template <typename ValueType>
using Decimeter		= Length<ValueType, Deci>;

template <typename ValueType>
using meter			= Length<ValueType>;

template <typename ValueType>
using Decameter		= Length<ValueType, Deca>;

template <typename ValueType>
using Hectometer	= Length<ValueType, Hecto>;

template <typename ValueType>
using Kilometer		= Length<ValueType, Kilo>;

namespace literals
{

constexpr Millimeter<double> operator""_mm(const long double value)
{
	return value;
}

constexpr Millimeter<std::size_t> operator""_mm(const unsigned long long int value)
{
	return value;
}

constexpr Centimeter<double> operator""_cm(const long double value)
{
	return value;
}

constexpr Centimeter<std::size_t> operator""_cm(const unsigned long long int value)
{
	return value;
}

constexpr Length<double> operator""_m(const long double value)
{
	return value;
}

constexpr Length<std::size_t> operator""_m(const unsigned long long int value)
{
	return value;
}

constexpr Kilometer<double> operator""_km(const long double value)
{
	return value;
}

constexpr Kilometer<std::size_t> operator""_km(const unsigned long long int value)
{
	return value;
}

} // namespace nd::math::units::literals

} // namespace nd::math::units

#endif // ND_MATH_UNITS_METER_HPP

