#ifndef ND_MATH_UNITS_INCH_HPP
#define ND_MATH_UNITS_INCH_HPP

#include "length.hpp"
#include "ratio.hpp"

namespace nd::math::units
{

template <typename ValueType>
using Inch			= Length<ValueType, RatioNormalize<Ratio<254u, 10'000u>>>;

template <typename ValueType>
using Thou			= Length<ValueType, RatioDiv<typename Inch<ValueType>::length_dimension_type::ratio_type, Ratio<1'000u>>>;

template <typename ValueType>
using Mil			= Thou<ValueType>;

template <typename ValueType>
using Foot			= Length<ValueType, RatioMul<typename Inch<ValueType>::length_dimension_type::ratio_type, Ratio<3u>>>;

template <typename ValueType>
using Yard			= Length<ValueType, RatioMul<typename Foot<ValueType>::length_dimension_type::ratio_type, Ratio<3u>>>;

template <typename ValueType>
using Mile			= Length<ValueType, RatioMul<typename Foot<ValueType>::length_dimension_type::ratio_type, Ratio<5'280u>>>;

template <typename ValueType>
using NauticalMile	= Length<ValueType, Ratio<1'852u>>;

namespace literals
{

constexpr Inch<double> operator""_in(const long double value)
{
	return value;
}

constexpr Inch<std::size_t> operator""_in(const unsigned long long int value)
{
	return value;
}

constexpr Thou<double> operator""_thou(const long double value)
{
	return value;
}

constexpr Thou<std::size_t> operator""_thou(const unsigned long long int value)
{
	return value;
}

constexpr Mil<double> operator""_mil(const long double value)
{
	return value;
}

constexpr Mil<std::size_t> operator""_mil(const unsigned long long int value)
{
	return value;
}

constexpr Foot<double> operator""_ft(const long double value)
{
	return value;
}

constexpr Foot<std::size_t> operator""_ft(const unsigned long long int value)
{
	return value;
}

constexpr Yard<double> operator""_yd(const long double value)
{
	return value;
}

constexpr Yard<std::size_t> operator""_yd(const unsigned long long int value)
{
	return value;
}

constexpr Mile<double> operator""_mi(const long double value)
{
	return value;
}

constexpr Mile<std::size_t> operator""_mi(const unsigned long long int value)
{
	return value;
}

constexpr NauticalMile<double> operator""_nmi(const long double value)
{
	return value;
}

constexpr NauticalMile<std::size_t> operator""_nmi(const unsigned long long int value)
{
	return value;
}

} // namespace nd::math::units::literals

} // namespace nd::math::units

#endif // ND_MATH_UNITS_INCH_HPP
