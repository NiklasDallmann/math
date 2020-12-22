#ifndef ND_MATH_DETAIL_HPP
#define ND_MATH_DETAIL_HPP

#include "common.hpp"
#include "traits.hpp"

namespace nd::math
{

template <
	typename ValueType,
	std::size_t			Columns>
class MatrixRowView;

template <
	typename ValueType,
	std::size_t			Rows,
	std::size_t			Columns>
class Matrix;

namespace detail
{

template <
	std::size_t			Columns,
	typename ValueType>
inline ValueType *rowPointer(ValueType *data, const std::size_t index)
{
	return (data + index * Columns);
}

//template <
//	typename ValueType,
//	std::size_t			Rows,
//	std::size_t			Columns>
//Matrix<ValueType, Rows, Columns> gaussJordan(const Matrix<ValueType, Rows, Columns> &matrix)
//{
//	Matrix<ValueType, Rows, Columns> returnValue;

//	// TODO

//	return returnValue;
//}

template <
	typename ValueType,
	std::size_t			Rows,
	std::size_t			Columns>
struct SubscriptType
{
	using value = MatrixRowView<ValueType, Columns>;
};

template <
	typename ValueType,
	std::size_t			Rows>
struct SubscriptType<ValueType, Rows, 1u>
{
	using value = ValueType &;
};

template <
	typename ValueType,
	std::size_t			Columns>
struct SubscriptType<ValueType, 1u, Columns>
{
	using value = ValueType &;
};

template <
	typename ValueType>
struct SubscriptType<ValueType, 1u, 1u>
{
	using value = ValueType &;
};

template <
	typename ValueType,
	std::size_t			Rows,
	std::size_t			Columns>
using SubscriptType_v = typename SubscriptType<ValueType, Rows, Columns>::value;

template <
	typename ValueType,
	std::size_t			Rows,
	std::size_t			Columns>
inline constexpr SubscriptType_v<ValueType, Rows, Columns> subscript(ValueType *data, const std::size_t index)
{
	if constexpr ((Rows == 1u) | (Columns == 1u))
	{
		return data[index];
	}
	else
	{
		return MatrixRowView<ValueType, Columns>{detail::rowPointer<Columns>(data, index)};
	}
}

template <
	typename ValueType,
	std::size_t			Rows,
	std::size_t			Columns>
inline constexpr ValueType &unifiedSubscript(Matrix<ValueType, Rows, Columns> &matrix, const std::size_t i, const std::size_t j)
{
	if constexpr ((Rows == 1u) | (Columns == 1u))
	{
		return matrix[i * Columns + j * Rows];
	}
	else
	{
		return matrix[i][j];
	}
}

template <
	typename ValueType,
	std::size_t			Rows,
	std::size_t			Columns>
inline constexpr ValueType unifiedSubscript(const Matrix<ValueType, Rows, Columns> &matrix, const std::size_t i, const std::size_t j)
{
	if constexpr ((Rows == 1u) | (Columns == 1u))
	{
		return matrix[i * Columns + j * Rows];
	}
	else
	{
		return matrix[i][j];
	}
}

} // namespace detail

} // namespace nd::math

#endif // ND_MATH_DETAIL_HPP
