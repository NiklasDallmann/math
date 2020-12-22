#ifndef ND_MATH_UNITS_LENGTH_HPP
#define ND_MATH_UNITS_LENGTH_HPP

#include "ratio.hpp"
#include "quantity.hpp"

namespace nd::math::units
{

template <typename ValueType, typename RatioType = Ratio<>>
using Length = Quantity<ValueType, Dimension<RatioType, 1>>;

} // namespace nd::math::units

#endif // ND_MATH_UNITS_LENGTH_HPP
