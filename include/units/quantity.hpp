#ifndef ND_MATH_UNITS_QUANTITY_HPP
#define ND_MATH_UNITS_QUANTITY_HPP

#include <cstdint>

#include "common.hpp"
#include "number.hpp"
#include "ratio.hpp"
#include "traits.hpp"

namespace nd::math::units
{

template <typename DimensionalRatio = Ratio<>, std::intmax_t DimensionalExponent = 0u>
struct Dimension
{
	using							ratio_type	= DimensionalRatio;
	static constexpr std::intmax_t	exponent	= DimensionalExponent;
};

template <typename DimensionalRatio, std::intmax_t DimensionalExponent>
std::ostream &operator<<(std::ostream &stream, [[maybe_unused]] const Dimension<DimensionalRatio, DimensionalExponent> dimension)
{
	stream << "Dimension<" << DimensionalRatio{} << ", " << DimensionalExponent << ">";
	return stream;
}

template <typename ValueType,
		  typename LengthDimension				= Dimension<>,
		  typename MassDimension				= Dimension<>,
		  typename TimeDimension				= Dimension<>,
		  typename ElectricCurrentDimension		= Dimension<>,
		  typename TemperatureDimension			= Dimension<>,
		  typename SubstanceAmountDimension		= Dimension<>,
		  typename LuminousIntensityDimension	= Dimension<>,
		  typename FactorRatio					= Ratio<>,
		  typename OffsetRatio					= Ratio<0u, 1u>>
class Quantity : public Number<ValueType>
{
public:
	using value_type						= ValueType;
	using length_dimension_type				= LengthDimension;
	using mass_dimension_type				= MassDimension;
	using time_dimension_type				= TimeDimension;
	using electric_current_dimension_type	= ElectricCurrentDimension;
	using temperature_dimension_type		= TemperatureDimension;
	using substance_amount_dimension_type	= SubstanceAmountDimension;
	using luminous_intensity_dimension_type	= LuminousIntensityDimension;
	using factor_type						= FactorRatio;
	using offset_type						= OffsetRatio;

	constexpr Quantity() = default;

	constexpr Quantity(const ValueType value) : Number<ValueType>{value}
	{
	}

private:
};

template <typename QuantityTypeTo, typename QuantityTypeFrom>
using HasEqualExponents = std::conjunction<
							traits::IsEqualIntMax<
								QuantityTypeFrom::length_dimension_type::exponent,				QuantityTypeTo::length_dimension_type::exponent>,
							traits::IsEqualIntMax<
								QuantityTypeFrom::mass_dimension_type::exponent,				QuantityTypeTo::mass_dimension_type::exponent>,
							traits::IsEqualIntMax<
								QuantityTypeFrom::time_dimension_type::exponent,				QuantityTypeTo::time_dimension_type::exponent>,
							traits::IsEqualIntMax<
								QuantityTypeFrom::electric_current_dimension_type::exponent,	QuantityTypeTo::electric_current_dimension_type::exponent>,
							traits::IsEqualIntMax<
								QuantityTypeFrom::temperature_dimension_type::exponent,			QuantityTypeTo::temperature_dimension_type::exponent>,
							traits::IsEqualIntMax<
								QuantityTypeFrom::substance_amount_dimension_type::exponent,	QuantityTypeTo::substance_amount_dimension_type::exponent>,
							traits::IsEqualIntMax<
								QuantityTypeFrom::luminous_intensity_dimension_type::exponent,	QuantityTypeTo::luminous_intensity_dimension_type::exponent>>;

template <typename QuantityTypeTo, typename QuantityTypeFrom>
using EnableEqualExponents = std::enable_if_t<HasEqualExponents<QuantityTypeTo, QuantityTypeFrom>::value>;

template <typename QuantityTypeTo, typename QuantityTypeFrom, typename = EnableEqualExponents<QuantityTypeTo, QuantityTypeFrom>>
constexpr QuantityTypeTo quantity_cast(const QuantityTypeFrom quantity)
{
	QuantityTypeTo returnValue;

	// Value types
	using ValueTypeTo	= typename QuantityTypeTo::value_type;

	// From dimension types
	using LengthDimensionFrom				= typename QuantityTypeFrom::length_dimension_type;
	using MassDimensionFrom					= typename QuantityTypeFrom::mass_dimension_type;
	using TimeDimensionFrom					= typename QuantityTypeFrom::time_dimension_type;
	using ElectricCurrentDimensionFrom		= typename QuantityTypeFrom::electric_current_dimension_type;
	using TemperatureDimensionFrom			= typename QuantityTypeFrom::temperature_dimension_type;
	using SubstanceAmountDimensionFrom		= typename QuantityTypeFrom::substance_amount_dimension_type;
	using LuminousIntensityDimensionFrom	= typename QuantityTypeFrom::luminous_intensity_dimension_type;

	// To dimension types
	using LengthDimensionTo					= typename QuantityTypeTo::length_dimension_type;
	using MassDimensionTo					= typename QuantityTypeTo::mass_dimension_type;
	using TimeDimensionTo					= typename QuantityTypeTo::time_dimension_type;
	using ElectricCurrentDimensionTo		= typename QuantityTypeTo::electric_current_dimension_type;
	using TemperatureDimensionTo			= typename QuantityTypeTo::temperature_dimension_type;
	using SubstanceAmountDimensionTo		= typename QuantityTypeTo::substance_amount_dimension_type;
	using LuminousIntensityDimensionTo		= typename QuantityTypeTo::luminous_intensity_dimension_type;

	// Other ratios
	using FactorRatioFrom					= typename QuantityTypeFrom::factor_type;
	using FactorRatioTo						= typename QuantityTypeTo::factor_type;

	using ConversionRatio = RatioMul<
		RatioPow<
			RatioDiv<
				typename LengthDimensionFrom::ratio_type,
				typename LengthDimensionTo::ratio_type>,
			LengthDimensionFrom::exponent>,
		RatioPow<
			RatioDiv<
				typename MassDimensionFrom::ratio_type,
				typename MassDimensionTo::ratio_type>,
			MassDimensionFrom::exponent>,
		RatioPow<
			RatioDiv<
				typename TimeDimensionFrom::ratio_type,
				typename TimeDimensionTo::ratio_type>,
			TimeDimensionFrom::exponent>,
		RatioPow<
			RatioDiv<
				typename ElectricCurrentDimensionFrom::ratio_type,
				typename ElectricCurrentDimensionTo::ratio_type>,
			ElectricCurrentDimensionFrom::exponent>,
		RatioPow<
			RatioDiv<
				typename TemperatureDimensionFrom::ratio_type,
				typename TemperatureDimensionTo::ratio_type>,
			TemperatureDimensionFrom::exponent>,
		RatioPow<
			RatioDiv<
				typename SubstanceAmountDimensionFrom::ratio_type,
				typename SubstanceAmountDimensionTo::ratio_type>,
			SubstanceAmountDimensionFrom::exponent>,
		RatioPow<
			RatioDiv<
				typename LuminousIntensityDimensionFrom::ratio_type,
				typename LuminousIntensityDimensionTo::ratio_type>,
			LuminousIntensityDimensionFrom::exponent>,
		RatioDiv<
			FactorRatioFrom,
			FactorRatioTo>>;

	// FIXME Take offset ratio into account

	constexpr ValueTypeTo conversionFactor = static_cast<ValueTypeTo>(ConversionRatio::numerator) / static_cast<ValueTypeTo>(ConversionRatio::denominator);

	returnValue = static_cast<ValueTypeTo>(quantity) * conversionFactor;

	return returnValue;
}

} // namespace nd::math::units

#endif // ND_MATH_UNITS_QUANTITY_HPP
