#ifndef ND_MATH_NUMBER_HPP
#define ND_MATH_NUMBER_HPP

#include <cstddef>

#include "traits.hpp"

namespace nd::math
{

template <typename ValueType, typename SizeType = std::size_t>
class Number
{
public:
	constexpr Number() = default;
	constexpr Number(const Number &other) = default;
	constexpr Number(Number &&other) = default;

	constexpr explicit Number(const ValueType value) :
		_value(value)
	{
	}

	// + - * / % ^ & | ~ ! = < > += -= *= /= %= ^= &= |= << >> >>= <<= == != <= >= <=> (since C++20) && || ++ -- , ->* -> ( ) [ ]
	// # # # # # # # # # # # # # ## ## ## ## ## ## ## ## ## ## ### ### ## ## ## ##                   ## ## ## ##

	constexpr operator ValueType() const
	{
		return this->_value;
	}

	constexpr Number &operator=(const Number &other) = default;
	constexpr Number &operator=(Number &&other) = default;

	constexpr Number &operator=(const ValueType &value)
	{
		this->_value = value;
		return *this;
	}

	constexpr Number &operator=(ValueType &&value)
	{
		this->_value = value;
		return *this;
	}

	constexpr Number &operator+=(const Number other)
	{
		this->_value += other._value;
		return *this;
	}

	constexpr Number &operator-=(const Number other)
	{
		this->_value -= other._value;
		return *this;
	}

	constexpr Number &operator*=(const Number other)
	{
		this->_value *= other._value;
		return *this;
	}

	constexpr Number &operator/=(const Number other)
	{
		this->_value /= other._value;
		return *this;
	}

	constexpr Number &operator%=(const Number other)
	{
		this->_value %= other._value;
		return *this;
	}

	constexpr Number &operator^=(const Number other)
	{
		this->_value ^= other._value;
		return *this;
	}

	constexpr Number &operator&=(const Number other)
	{
		this->_value &= other._value;
		return *this;
	}

	constexpr Number &operator|=(const Number other)
	{
		this->_value |= other._value;
		return *this;
	}

	constexpr Number &operator<<=(const SizeType bits)
	{
		this->_value <<= bits;
		return *this;
	}

	constexpr Number &operator>>=(const SizeType bits)
	{
		this->_value <<= bits;
		return *this;
	}

	constexpr bool operator==(const Number other) const
	{
		return (this->_value == other._value);
	}

	constexpr bool operator!=(const Number other) const
	{
		return (this->_value != other._value);
	}

	constexpr bool operator>(const Number other) const
	{
		return (this->_value > other._value);
	}

	constexpr bool operator<(const Number other) const
	{
		return (this->_value < other._value);
	}

	constexpr bool operator>=(const Number other) const
	{
		return (this->_value >= other._value);
	}

	constexpr bool operator<=(const Number other) const
	{
		return (this->_value <= other._value);
	}

	constexpr Number &operator++()
	{
		++this->_value;
		return *this;
	}

	constexpr Number &operator--()
	{
		--this->_value;
		return *this;
	}

	constexpr Number operator++(int)
	{
		this->_value++;
		return *this;
	}

	constexpr Number operator--(int)
	{
		this->_value--;
		return *this;
	}

	constexpr Number operator+() const
	{
		return +this->_value;
	}

	constexpr Number operator-() const
	{
		return -this->_value;
	}

	constexpr Number operator+(const Number other) const
	{
		return (this->_value + other._value);
	}

	constexpr Number operator-(const Number other) const
	{
		return (this->_value - other._value);
	}

	constexpr Number operator*(const Number other) const
	{
		return (this->_value * other._value);
	}

	constexpr Number operator/(const Number other) const
	{
		return (this->_value / other._value);
	}

	constexpr Number operator%(const Number other) const
	{
		return (this->_value % other._value);
	}

	constexpr Number operator~() const
	{
		return ~this->_value;
	}

	constexpr Number operator&(const Number other) const
	{
		return (this->_value & other._value);
	}

	constexpr Number operator|(const Number other) const
	{
		return (this->_value | other._value);
	}

	constexpr Number operator^(const Number other) const
	{
		return (this->_value ^ other._value);
	}

	constexpr Number operator<<(const SizeType bits) const
	{
		return (this->_value << bits);
	}

	constexpr Number operator>>(const SizeType bits) const
	{
		return (this->_value >> bits);
	}

	constexpr bool operator!() const
	{
		return !this->_value;
	}

	constexpr Number operator&&(const Number other) const
	{
		return (this->_value && other._value);
	}

	constexpr Number operator||(const Number other) const
	{
		return (this->_value || other._value);
	}

protected:
	ValueType _value;
};

} // namespace nd::math

#endif // ND_MATH_NUMBER_HPP
