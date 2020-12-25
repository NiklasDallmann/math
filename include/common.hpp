#ifndef ND_MATH_COMMON_HPP
#define ND_MATH_COMMON_HPP

#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <limits>

#include "traits.hpp"

#define ND_ALWAYS_INLINE __attribute__ ((always_inline))

namespace nd::math::common
{

template <typename ValueType>
inline constexpr void setZero(ValueType * const data, const std::size_t count)
{
	if (std::is_constant_evaluated())
	{
		for (std::size_t index = 0u; index < count; ++index)
		{
			data[index] = {};
		}
	}
	else
	{
		std::memset(data, 0, count * sizeof (ValueType));
	}
}

template <typename ValueType>
inline constexpr void copy(ValueType * const destination, const ValueType * const source, const std::size_t count)
{
	if (std::is_constant_evaluated())
	{
		for (std::size_t index = 0u; index < count; ++index)
		{
			destination[index] = source[index];
		}
	}
	else
	{
		std::memcpy(destination, source, count * sizeof (ValueType));
	}
}

template <typename ValueType>
inline bool isEqual(const ValueType * const left, const ValueType * const right, const size_t count)
{
	return (std::memcmp(left, right, count * sizeof (ValueType)) == 0);
}

template <typename ValueType>
inline constexpr ValueType pow2(const ValueType value)
{
	return (value * value);
}

template <typename ValueType>
inline constexpr bool isEqual(const ValueType left, const ValueType right, const ValueType epsilon)
{
	using std::abs;
	return (abs(left - right) < epsilon);
}

template <typename ValueType>
inline constexpr bool isEqual(const ValueType left, const ValueType right)
{
	constexpr ValueType epsilon = std::numeric_limits<ValueType>::min();
	return isEqual(left, right, epsilon);
}

template <typename ValueType>
inline constexpr bool isPositive(const ValueType value)
{
	return (value >= static_cast<ValueType>(0));
}

template <typename ValueType>
inline constexpr ValueType pow(const ValueType value, const std::intmax_t exponent)
{
	if (exponent >= 0)
	{
		return (exponent == 0) ? static_cast<ValueType>(1) : value * pow(value, exponent - 1);
	}
	else
	{
		return (static_cast<ValueType>(1) / value) / pow(-value, exponent);
	}
}

}

#endif // ND_MATH_COMMON_HPP
