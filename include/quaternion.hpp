#ifndef ND_MATH_QUATERNION_HPP
#define ND_MATH_QUATERNION_HPP

#include <cassert>
#include <cmath>
#include <cstddef>
#include <string>

#include "units/angle.hpp"
#include "matrix.hpp"
#include "traits.hpp"

namespace nd::math
{

template <typename ValueType>
class Quaternion
{
public:
	constexpr Quaternion() = default;

	constexpr Quaternion(const ValueType a, const ValueType b, const ValueType c, const ValueType d) :
		_data({a, b, c, d})
	{
	}

	constexpr Quaternion(const Vector3_f &vector) :
		_data({{}, vector})
	{
	}

	constexpr Quaternion(const units::Radians<ValueType> angle, const Vector3_f &axis)
	{
		using std::cos;
		using std::sin;

		const ValueType halfAngle = static_cast<ValueType>(angle) / static_cast<ValueType>(2);

		this->_data = Vector4<ValueType>{cos(halfAngle), sin(halfAngle) * axis.normalized()};
	}

	constexpr Quaternion(const traits::initialization::Zero) :
		_data(traits::initialization::zero)
	{
	}

	constexpr Quaternion &conjugate()
	{
		constexpr ValueType factors[Quaternion::_size] = {1, -1, -1, -1};

		for (std::size_t index = 0u; index < Quaternion::_size; ++index)
		{
			(*this)[index] = (*this)[index] * factors[index];
		}

		return *this;
	}

	constexpr Quaternion conjugated() const
	{
		Quaternion returnValue = *this;
		returnValue.conjugate();
		return returnValue;
	}

	constexpr ValueType norm() const
	{
		return this->_data.norm();
	}

	constexpr Quaternion &normalize()
	{
		this->_data.normalize();
		return *this;
	}

	constexpr Quaternion normalized() const
	{
		Quaternion returnValue = *this;
		returnValue.normalize();
		return returnValue;
	}

	constexpr bool isNormalized() const
	{
		return common::isEqual(this->norm(), static_cast<ValueType>(1));
	}

	constexpr Quaternion &invert()
	{
		this->conjugate();
		this->_data /= this->_data.squareNorm();
		return *this;
	}

	constexpr Quaternion inverted() const
	{
		Quaternion returnValue = *this;
		returnValue.invert();
		return returnValue;
	}

	constexpr Matrix4x4<ValueType> toRotationMatrix() const
	{
		assert(this->isNormalized());
		constexpr ValueType zero	= static_cast<ValueType>(0);
		constexpr ValueType one		= static_cast<ValueType>(1);
		constexpr ValueType two		= static_cast<ValueType>(2);

		// FIXME Optimize
		Matrix4x4<ValueType> returnValue
		{
			one - two * (common::pow2(this->_data[2u]) + common::pow2(this->_data[3u])),
			two * (this->_data[1u] * this->_data[2u] - this->_data[3u] * this->_data[0u]),
			two * (this->_data[1u] * this->_data[3u] + this->_data[2u] * this->_data[0u]),
			zero,

			two * (this->_data[1u] * this->_data[2u] + this->_data[3u] * this->_data[0u]),
			one - two * (common::pow2(this->_data[1u]) + common::pow2(this->_data[3u])),
			two * (this->_data[2u] * this->_data[3u] - this->_data[1u] * this->_data[0u]),
			zero,

			two * (this->_data[1u] * this->_data[3u] - this->_data[2u] * this->_data[0u]),
			two * (this->_data[2u] * this->_data[3u] + this->_data[1u] * this->_data[0u]),
			one - two * (common::pow2(this->_data[1u]) + common::pow2(this->_data[2u])),
			zero,

			zero,
			zero,
			zero,
			one
		};

		return returnValue;
	}

	constexpr ValueType &operator[](const std::size_t index)
	{
		return this->_data[index];
	}

	constexpr ValueType operator[](const std::size_t index) const
	{
		return this->_data[index];
	}

	constexpr Quaternion &operator+=(const Quaternion &other)
	{
		return (this->_data += other._data);
	}

	constexpr Quaternion &operator-=(const Quaternion &other)
	{
		return (this->_data -= other._data);
	}

	constexpr Quaternion &operator*=(const ValueType scalar)
	{
		return (this->_data *= scalar);
	}

	constexpr Quaternion &operator*=(const Quaternion &other)
	{
		*this = *this * other;
		return *this;
	}

	constexpr Quaternion &operator/=(const ValueType scalar)
	{
		return (this->_data /= scalar);
	}

	constexpr Quaternion operator+(const Quaternion &other) const
	{
		Quaternion returnValue = *this;
		returnValue += other;
		return returnValue;
	}

	constexpr Quaternion operator-(const Quaternion &other) const
	{
		Quaternion returnValue = *this;
		returnValue -= other;
		return returnValue;
	}

	constexpr Quaternion operator*(const ValueType scalar) const
	{
		Quaternion returnValue = *this;
		returnValue *= scalar;
		return returnValue;
	}

	constexpr Quaternion operator*(const Quaternion &other) const
	{
		Quaternion returnValue = *this;

		returnValue[0u] = (*this)[0u] * other[0u] - (*this)[1u] * other[1u] - (*this)[2u] * other[2u] - (*this)[3u] * other[3u];
		returnValue[1u] = (*this)[0u] * other[1u] + (*this)[1u] * other[0u] + (*this)[2u] * other[3u] - (*this)[3u] * other[2u]; // i
		returnValue[2u] = (*this)[0u] * other[2u] - (*this)[1u] * other[3u] + (*this)[2u] * other[0u] + (*this)[3u] * other[1u]; // j
		returnValue[3u] = (*this)[0u] * other[3u] + (*this)[1u] * other[2u] - (*this)[2u] * other[1u] + (*this)[3u] * other[0u]; // k

		return returnValue;
	}

	constexpr Quaternion operator/(const ValueType scalar) const
	{
		Quaternion returnValue = *this;
		returnValue /= scalar;
		return returnValue;
	}

	constexpr operator Vector4<ValueType>() const
	{
		return this->_data;
	}

	constexpr operator Vector3<ValueType>() const
	{
		Vector3_f returnValue{traits::initialization::zero};
		common::copy(returnValue.data(), this->_data.data() + 1u, Quaternion::_size - 1u);
		return returnValue;
	}

private:
	static constexpr std::size_t _size = 4u;

	Vector4<ValueType> _data;
};

template <typename ValueType>
std::ostream &operator<<(std::ostream &stream, const Quaternion<ValueType> &quaternion)
{
	stream << std::to_string(quaternion[0u]) << " + " << std::to_string(quaternion[1u]) << "i + " << std::to_string(quaternion[2u]) << "j + "
		   << std::to_string(quaternion[3u]) << "k";
	return stream;
}

using Quaternion_f		= Quaternion<float>;
using Quaternion_d		= Quaternion<double>;
using Quaternion_i64	= Quaternion<std::int64_t>;
using Quaternion_i32	= Quaternion<std::int32_t>;
using Quaternion_i16	= Quaternion<std::int16_t>;
using Quaternion_i8		= Quaternion<std::int8_t>;
using Quaternion_u64	= Quaternion<std::uint64_t>;
using Quaternion_u32	= Quaternion<std::uint32_t>;
using Quaternion_u16	= Quaternion<std::uint16_t>;
using Quaternion_u8		= Quaternion<std::uint8_t>;

} // namespace nd::math

#endif // ND_MATH_QUATERNION_HPP
