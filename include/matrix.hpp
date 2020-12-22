#ifndef ND_MATH_MATRIX_HPP
#define ND_MATH_MATRIX_HPP

#include <array>
#include <algorithm>
#include <cassert>
#include <cmath>
#include <cstddef>
#include <cstdint>
#include <iomanip>
#include <limits>
#include <ostream>
#include <type_traits>

#include "common.hpp"
#include "traits.hpp"
#include "detail.hpp"

namespace nd::math
{

template <
	typename ValueType,
	std::size_t			Columns>
class MatrixRowView
{
public:
	MatrixRowView(ValueType *data) :
		_data(const_cast<ValueType *>(data))
	{
	}

	constexpr ValueType &operator[](const std::size_t index)
	{
		assert(index < Columns);
		return this->_data[index];
	}

	constexpr ValueType operator[](const std::size_t index) const
	{
		assert(index < Columns);
		return this->_data[index];
	}

	constexpr ValueType *data()
	{
		return this->_data;
	}

	constexpr const ValueType *data() const
	{
		return const_cast<ValueType const * const>(this->_data);
	}

private:
	ValueType *_data = nullptr;
};

template <
	typename ValueType,
	std::size_t			Rows,
	std::size_t			Columns>
class Matrix
{
public:
	constexpr Matrix() = default;

	constexpr Matrix(const traits::initialization::Zero)
	{
		common::setZero(this->_data, Rows * Columns);
	}

	template <typename Unused_ = void, typename = traits::EnableEqualSize<Rows, Columns, Unused_>>
	constexpr Matrix(const traits::initialization::Identity)
	{
		common::setZero(this->_data, Rows * Columns);

		for (std::size_t order = 0u; order < Rows; ++order)
		{
			(*this)[order][order] = static_cast<ValueType>(1);
		}
	}

	constexpr Matrix(const std::array<ValueType, Rows * Columns> &values)
	{
		common::copy(this->_data, values.data(), Rows * Columns);
	}

	template <typename Unused_ = void, typename = traits::EnableVector<Rows, Columns, Unused_>>
	constexpr Matrix(const ValueType scalar, const Matrix<ValueType, 1u, Columns - 1u> &vector)
	{
		this->_data[0u] = scalar;
		common::copy(&this->_data[1u], vector.data(), Columns - 1u);
	}

	template <typename Unused_ = void, typename = traits::EnableVector<Rows, Columns, Unused_>>
	constexpr Matrix(const ValueType scalar, const Matrix<ValueType, Rows - 1u, 1u> &vector)
	{
		this->_data[0u] = scalar;
		common::copy(&this->_data[1u], vector.data(), Rows - 1u);
	}

	constexpr Matrix(const std::array<Matrix<ValueType, 1u, Columns>, Rows> rows)
	{
		for (std::size_t rowIndex = 0u; rowIndex < Rows; ++rowIndex)
		{
			common::copy(detail::rowPointer(this->_data, rowIndex), rows[rowIndex].data());
		}
	}

	constexpr ValueType *data()
	{
		return this->_data;
	}

	constexpr const ValueType *data() const
	{
		return const_cast<ValueType const * const>(this->_data);
	}

	constexpr void setZero()
	{
		common::setZero(this->_data, Rows * Columns);
	}

	constexpr Matrix<ValueType, Columns, Rows> transposed() const
	{
		Matrix<ValueType, Columns, Rows> returnValue;

		for (std::size_t i = 0u; i < Rows; ++i)
		{
			for (std::size_t j = 0u; j < Columns; ++j)
			{
				detail::unifiedSubscript(returnValue, j, i) = detail::unifiedSubscript(*this, i, j);
			}
		}

		return returnValue;
	}

	template <typename Unused_ = void, typename = traits::EnableVector<Rows, Columns, Unused_>>
	constexpr ValueType squareNorm() const
	{
		ValueType returnValue = {};

		for (std::size_t index = 0u; index < (Rows * Columns); ++index)
		{
			returnValue += common::pow2((*this)[index]);
		}

		return returnValue;
	}

	template <typename Unused_ = void, typename = traits::EnableVector<Rows, Columns, Unused_>>
	constexpr ValueType norm() const
	{
		using std::sqrt;
		return sqrt(this->squareNorm());
	}

	template <typename Unused_ = void, typename = traits::EnableVector<Rows, Columns, Unused_>>
	constexpr Matrix &normalize()
	{
		*this *= this->norm();
		return *this;
	}

	template <typename Unused_ = void, typename = traits::EnableVector<Rows, Columns, Unused_>>
	constexpr Matrix normalized() const
	{
		Matrix returnValue = *this;
		returnValue.normalize();
		return returnValue;
	}

	template <typename Unused_ = void, typename = traits::Enable3DVector<Rows, Columns, Unused_>>
	constexpr Matrix cross(const Matrix &other) const
	{
		Matrix returnValue;

		returnValue[0u] = (*this)[1u] * other[2u] - (*this)[2u] * other[1u];
		returnValue[1u] = (*this)[2u] * other[0u] - (*this)[0u] * other[2u];
		returnValue[2u] = (*this)[0u] * other[1u] - (*this)[1u] * other[0u];

		return returnValue;
	}

	constexpr auto operator[](const std::size_t index) -> decltype (detail::subscript<ValueType, Rows, Columns>(nullptr, {}))
	{
		assert(index < Rows);
		return detail::subscript<ValueType, Rows, Columns>(this->_data, index);
	}

	constexpr auto operator[](const std::size_t index) const -> std::add_const_t<decltype (detail::subscript<ValueType, Rows, Columns>(nullptr, {}))>
	{
		assert(index < Rows);
		return detail::subscript<ValueType, Rows, Columns>(this->_data, index);
	}

	constexpr bool operator==(const Matrix &other) const
	{
		return common::isEqual(this->_data, other._data, Rows * Columns);
	}

	constexpr bool operator!=(const Matrix &other) const
	{
		return !(*this == other);
	}

	constexpr Matrix &operator+=(const Matrix &other)
	{
		for (std::size_t i = 0u; i < Rows; ++i)
		{
			for (std::size_t j = 0u; j < Columns; ++j)
			{
				detail::unifiedSubscript(*this, i, j) += detail::unifiedSubscript(other, i, j);
			}
		}

		return *this;
	}

	constexpr Matrix &operator-=(const Matrix &other)
	{
		for (std::size_t i = 0u; i < Rows; ++i)
		{
			for (std::size_t j = 0u; j < Columns; ++j)
			{
				detail::unifiedSubscript(*this, i, j) -= detail::unifiedSubscript(other, i, j);
			}
		}

		return *this;
	}

	template <typename Unused_ = void, typename = traits::EnableEqualSize<Rows, Columns, Unused_>>
	constexpr Matrix &operator*=(const Matrix &other)
	{
		(*this) = (*this) * other;
		return *this;
	}

	constexpr Matrix &operator*=(const ValueType scalar)
	{
		for (std::size_t i = 0u; i < Rows * Columns; ++i)
		{
			this->_data[i] *= scalar;
		}

		return *this;
	}

	constexpr Matrix &operator/=(const ValueType scalar)
	{
		for (std::size_t i = 0u; i < Rows * Columns; ++i)
		{
			this->_data[i] /= scalar;
		}

		return *this;
	}

	constexpr Matrix operator+(const Matrix &other) const
	{
		Matrix returnValue = *this;
		returnValue += other;
		return returnValue;
	}

	constexpr Matrix operator-(const Matrix &other) const
	{
		Matrix returnValue = *this;
		returnValue -= other;
		return returnValue;
	}

	constexpr Matrix operator*(const ValueType scalar) const
	{
		Matrix returnValue = *this;
		returnValue *= scalar;
		return returnValue;
	}

	constexpr Matrix operator/(const ValueType scalar) const
	{
		Matrix returnValue = *this;
		returnValue /= scalar;
		return returnValue;
	}

	template <typename Unused_ = void, typename = traits::EnableNegative<ValueType, Unused_>>
	constexpr Matrix operator-() const
	{
		return (*this) * static_cast<ValueType>(-1);
	}

	template <typename Unused_ = void, typename = traits::EnableScalar<Rows, Columns, Unused_>>
	constexpr operator ValueType() const
	{
		return (*this)[0];
	}

	template <std::size_t TargetRows, std::size_t TargetColumns>
	constexpr operator Matrix<ValueType, TargetRows, TargetColumns>() const
	{
		Matrix<ValueType, TargetRows, TargetColumns> returnValue{traits::initialization::zero};

		for (std::size_t i = 0u; i < std::min(Rows, TargetRows); ++i)
		{
			for (std::size_t j = 0u; j < std::min(Columns, TargetColumns); ++j)
			{
				detail::unifiedSubscript(returnValue, i, j) = detail::unifiedSubscript(*this, i, j);
			}
		}

		return returnValue;
	}

protected:
	mutable ValueType _data[Rows * Columns];
};

template <typename ValueType, std::size_t Rows, std::size_t Columns>
constexpr Matrix<ValueType, Rows, Columns> operator*(const ValueType scalar, const Matrix<ValueType, Rows, Columns> &matrix)
{
	return (matrix * scalar);
}

template <typename ValueType, std::size_t L, std::size_t M, std::size_t N>
constexpr Matrix<ValueType, L, N> operator*(const Matrix<ValueType, L, M> &left, const Matrix<ValueType, M, N> &right)
{
	Matrix<ValueType, L, N> returnValue;

	mul(returnValue, left, right);

	return returnValue;
}

template <typename ValueType, std::size_t L, std::size_t M, std::size_t N>
constexpr void mul(Matrix<ValueType, L, N> &result, const Matrix<ValueType, L, M> &left, const Matrix<ValueType, M, N> &right)
{
	result.setZero();

	for (std::size_t i = 0u; i < L; ++i)
	{
		for (std::size_t k = 0u; k < M; ++k)
		{
			for (std::size_t j = 0u; j < N; ++j)
			{
				detail::unifiedSubscript(result, i, j) += detail::unifiedSubscript(left, i, k) * detail::unifiedSubscript(right, k, j);
			}
		}
	}
}

template <typename ValueType, std::size_t Rows, std::size_t Columns>
std::ostream &operator<<(std::ostream &stream, const Matrix<ValueType, Rows, Columns> &matrix)
{
	const std::size_t maxLength = [&matrix]()
	{
		std::size_t returnValue = 0u;

		for (std::size_t i = 0u; i < Rows; ++i)
		{
			for (std::size_t j = 0u; j < Columns; ++j)
			{
				returnValue = std::max(returnValue, std::to_string(detail::unifiedSubscript(matrix, i, j)).size());
			}
		}

		return returnValue;
	}();

	for (std::size_t i = 0u; i < Rows; ++i)
	{
		if (Rows == 1u)
		{
			stream << "(";
		}
		else
		{
			if (i == 0u)
			{
				stream << "\xE2\x8E\x9B";
			}
			else if (i == Rows - 1u)
			{
				stream << "\xE2\x8E\x9D";
			}
			else
			{
				stream << "\xE2\x8E\x9C";
			}
		}

		for (std::size_t j = 0u; j < Columns; ++j)
		{
			stream << std::setw(maxLength + 1u) << std::setfill(' ') << std::to_string(detail::unifiedSubscript(matrix, i, j)) << std::setw(0u) << " ";
		}

		if (Rows == 1u)
		{
			stream << ")";
		}
		else
		{
			if (i == 0u)
			{
				stream << "\xE2\x8E\x9E";
			}
			else if (i == Rows - 1u)
			{
				stream << "\xE2\x8E\xA0";
			}
			else
			{
				stream << "\xE2\x8E\x9F";
			}
		}

		if (i != Rows - 1)
		{
			stream << "\n";
		}
	}

	stream.setstate({});

	return stream;
}

// Matrices
template <typename T>
using Matrix4x4 = Matrix<T, 4u, 4u>;

template <typename T>
using Matrix3x3 = Matrix<T, 3u, 3u>;

template <typename T>
using Matrix2x2 = Matrix<T, 2u, 2u>;

// Vectors
template <typename T, std::size_t Order>
using Vector = Matrix<T, 1u, Order>;

template <typename T>
using Vector4 = Vector<T, 4u>;

template <typename T>
using Vector3 = Vector<T, 3u>;

template <typename T>
using Vector2 = Vector<T, 2u>;

// Column vectors
template <typename T, std::size_t Order>
using ColumnVector	= Matrix<T, Order, 1u>;

template <typename T>
using ColumnVector4 = ColumnVector<T, 4u>;

template <typename T>
using ColumnVector3 = ColumnVector<T, 3u>;

template <typename T>
using ColumnVector2 = ColumnVector<T, 2u>;

// Short matrix aliases
using Matrix4x4_f		= Matrix4x4<float>;
using Matrix3x3_f		= Matrix3x3<float>;
using Matrix2x2_f		= Matrix2x2<float>;

using Matrix4x4_d		= Matrix4x4<double>;
using Matrix3x3_d		= Matrix3x3<double>;
using Matrix2x2_d		= Matrix2x2<double>;

using Matrix4x4_i64		= Matrix4x4<std::int64_t>;
using Matrix3x3_i64		= Matrix3x3<std::int64_t>;
using Matrix2x2_i64		= Matrix2x2<std::int64_t>;

using Matrix4x4_i32		= Matrix4x4<std::int32_t>;
using Matrix3x3_i32		= Matrix3x3<std::int32_t>;
using Matrix2x2_i32		= Matrix2x2<std::int32_t>;

using Matrix4x4_i16		= Matrix4x4<std::int16_t>;
using Matrix3x3_i16		= Matrix3x3<std::int16_t>;
using Matrix2x2_i16		= Matrix2x2<std::int16_t>;

using Matrix4x4_i8		= Matrix4x4<std::int8_t>;
using Matrix3x3_i8		= Matrix3x3<std::int8_t>;
using Matrix2x2_i8		= Matrix2x2<std::int8_t>;

using Matrix4x4_u64		= Matrix4x4<std::uint64_t>;
using Matrix3x3_u64		= Matrix3x3<std::uint64_t>;
using Matrix2x2_u64		= Matrix2x2<std::uint64_t>;

using Matrix4x4_u32		= Matrix4x4<std::uint32_t>;
using Matrix3x3_u32		= Matrix3x3<std::uint32_t>;
using Matrix2x2_u32		= Matrix2x2<std::uint32_t>;

using Matrix4x4_u16		= Matrix4x4<std::uint16_t>;
using Matrix3x3_u16		= Matrix3x3<std::uint16_t>;
using Matrix2x2_u16		= Matrix2x2<std::uint16_t>;

using Matrix4x4_u8		= Matrix4x4<std::uint8_t>;
using Matrix3x3_u8		= Matrix3x3<std::uint8_t>;
using Matrix2x2_u8		= Matrix2x2<std::uint8_t>;

// Short row vector aliases
using Vector4_f			= Vector4<float>;
using Vector3_f			= Vector3<float>;
using Vector2_f			= Vector2<float>;

using Vector4_d			= Vector4<double>;
using Vector3_d			= Vector3<double>;
using Vector2_d			= Vector2<double>;

using Vector4_i64		= Vector4<std::int64_t>;
using Vector3_i64		= Vector3<std::int64_t>;
using Vector2_i64		= Vector2<std::int64_t>;

using Vector4_i32		= Vector4<std::int32_t>;
using Vector3_i32		= Vector3<std::int32_t>;
using Vector2_i32		= Vector2<std::int32_t>;

using Vector4_i16		= Vector4<std::int16_t>;
using Vector3_i16		= Vector3<std::int16_t>;
using Vector2_i16		= Vector2<std::int16_t>;

using Vector4_i8		= Vector4<std::int8_t>;
using Vector3_i8		= Vector3<std::int8_t>;
using Vector2_i8		= Vector2<std::int8_t>;

using Vector4_u64		= Vector4<std::uint64_t>;
using Vector3_u64		= Vector3<std::uint64_t>;
using Vector2_u64		= Vector2<std::uint64_t>;

using Vector4_u32		= Vector4<std::uint32_t>;
using Vector3_u32		= Vector3<std::uint32_t>;
using Vector2_u32		= Vector2<std::uint32_t>;

using Vector4_u16		= Vector4<std::uint16_t>;
using Vector3_u16		= Vector3<std::uint16_t>;
using Vector2_u16		= Vector2<std::uint16_t>;

using Vector4_u8		= Vector4<std::uint8_t>;
using Vector3_u8		= Vector3<std::uint8_t>;
using Vector2_u8		= Vector2<std::uint8_t>;

// Short column vector aliases
using ColumnVector4_f	= ColumnVector4<float>;
using ColumnVector3_f	= ColumnVector3<float>;
using ColumnVector2_f	= ColumnVector2<float>;

using ColumnVector4_d	= ColumnVector4<double>;
using ColumnVector3_d	= ColumnVector3<double>;
using ColumnVector2_d	= ColumnVector2<double>;

using ColumnVector4_i64	= ColumnVector4<std::int64_t>;
using ColumnVector3_i64	= ColumnVector3<std::int64_t>;
using ColumnVector2_i64	= ColumnVector2<std::int64_t>;

using ColumnVector4_i32	= ColumnVector4<std::int32_t>;
using ColumnVector3_i32	= ColumnVector3<std::int32_t>;
using ColumnVector2_i32	= ColumnVector2<std::int32_t>;

using ColumnVector4_i16	= ColumnVector4<std::int16_t>;
using ColumnVector3_i16	= ColumnVector3<std::int16_t>;
using ColumnVector2_i16	= ColumnVector2<std::int16_t>;

using ColumnVector4_i8	= ColumnVector4<std::int8_t>;
using ColumnVector3_i8	= ColumnVector3<std::int8_t>;
using ColumnVector2_i8	= ColumnVector2<std::int8_t>;

using ColumnVector4_u64	= ColumnVector4<std::uint64_t>;
using ColumnVector3_u64	= ColumnVector3<std::uint64_t>;
using ColumnVector2_u64	= ColumnVector2<std::uint64_t>;

using ColumnVector4_u32	= ColumnVector4<std::uint32_t>;
using ColumnVector3_u32	= ColumnVector3<std::uint32_t>;
using ColumnVector2_u32	= ColumnVector2<std::uint32_t>;

using ColumnVector4_u16	= ColumnVector4<std::uint16_t>;
using ColumnVector3_u16	= ColumnVector3<std::uint16_t>;
using ColumnVector2_u16	= ColumnVector2<std::uint16_t>;

using ColumnVector4_u8	= ColumnVector4<std::uint8_t>;
using ColumnVector3_u8	= ColumnVector3<std::uint8_t>;
using ColumnVector2_u8	= ColumnVector2<std::uint8_t>;

}

#endif // ND_MATH_MATRIX_HPP
