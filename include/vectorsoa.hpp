#ifndef ND_MATH_VECTOR_SOA_HPP
#define ND_MATH_VECTOR_SOA_HPP

#include <cstddef>
#include <vector>

#include "matrix.hpp"

namespace nd::math
{

template <typename ValueType, std::size_t Order>
class VectorSoA
{
public:
	using VectorType = ColumnVector<ValueType, Order>;

	constexpr VectorType operator[](const std::size_t index) const
	{
		VectorType returnValue;

		this->subscript<0u>(returnValue, index);

		return returnValue;
	}



private:
	struct SoA
	{
		std::vector<ValueType> data[Order];

		constexpr std::vector<ValueType> &operator[](const std::size_t order)
		{
			return data[order];
		}
	};

	SoA _data;

	template <std::size_t OrderIndex>
	constexpr void subscript(VectorType &vector, const std::size_t index)
	{
		vector[OrderIndex] = this->_data[OrderIndex][index];
		subscript(vector, index + 1u);
	}

	template <>
	constexpr void subscript<Order - 1u>(VectorType &vector, const std::size_t index)
	{
		vector[Order - 1u] = this->_data[Order - 1u][index];
	}
};

}

#endif // ND_MATH_VECTOR_SOA_HPP
