#ifndef ND_MATH_UNITS_MASS_HPP
#define ND_MATH_UNITS_MASS_HPP

#include "ratio.hpp"
#include "quantity.hpp"

namespace nd::math::units
{

template <typename ValueType, typename RatioType = Ratio<>>
using Mass = Quantity<ValueType, Dimension<>, Dimension<RatioType, 1>>;

} // namespace nd::math::units

#endif // ND_MATH_UNITS_MASS_HPP
