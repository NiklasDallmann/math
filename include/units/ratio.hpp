#ifndef ND_MATH_UNITS_RATIO_HPP
#define ND_MATH_UNITS_RATIO_HPP

#include <cstdint>
#include <cstdlib>
#include <numeric>
#include <ostream>
#include <ratio>

#include <common.hpp>

namespace nd::math::units
{

template <std::uintmax_t Numerator = 1u, std::uintmax_t Denominator = 1u>
struct Ratio
{
	using StdRatioType = std::ratio<Numerator, Denominator>;

	static constexpr std::uintmax_t numerator	= Numerator;
	static constexpr std::uintmax_t denominator	= Denominator;

	constexpr Ratio() = default;

	constexpr Ratio(StdRatioType)
	{
	}

	constexpr operator StdRatioType() const
	{
		return {};
	}
};

template <std::uintmax_t Numerator, std::uintmax_t Denominator>
std::ostream &operator<<(std::ostream &stream, [[maybe_unused]] const Ratio<Numerator, Denominator> ratio)
{
	stream << "Ratio<" << Numerator << ", " << Denominator << ">";
	return stream;
}

namespace detail
{

template <std::uintmax_t Numerator, std::uintmax_t Denominator>
struct RatioNormalize
{
	using type = Ratio<Numerator / std::gcd(Numerator, Denominator), Denominator / std::gcd(Numerator, Denominator)>;
};

template <std::uintmax_t LeftNum, std::uintmax_t RightNum, std::uintmax_t LeftDen, std::uintmax_t RightDen>
struct RatioMul
{
	using type = typename RatioNormalize<LeftNum * RightNum, LeftDen * RightDen>::type;
};

template <typename F0, typename ...Fs>
struct RatioMultiplyMultiple
{
private:
	using TailProduct = typename RatioMultiplyMultiple<Fs...>::type;

public:
	using type = typename RatioMul<F0::numerator, TailProduct::numerator, F0::denominator, TailProduct::denominator>::type;
};

template <typename F>
struct RatioMultiplyMultiple<F>
{
	using type = F;
};

template <std::uintmax_t LeftNum, std::uintmax_t RightNum, std::uintmax_t LeftDen, std::uintmax_t RightDen>
struct RatioDiv
{
	using type = typename RatioNormalize<LeftNum * RightDen, LeftDen * RightNum>::type;
};

} // namespace nd::math::units::detail

template <typename ...Factors>
using RatioMul = typename detail::RatioMultiplyMultiple<Factors...>::type;

template <typename Left, typename Right>
using RatioDiv = typename detail::RatioDiv<Left::numerator, Right::numerator, Left::denominator, Right::denominator>::type;

namespace detail
{

template <typename R, std::intmax_t Power, bool IsPositivePower>
struct RatioPow;

template <typename R, std::intmax_t Power>
struct RatioPow<R, Power, true>
{
	using type = units::RatioMul<R, typename RatioPow<R, Power - 1, true>::type>;
};

template <typename R, std::intmax_t Power>
struct RatioPow<R, Power, false>
{
	using type = units::RatioDiv<Ratio<>, typename RatioPow<R, -Power, true>::type>;
};

template <typename R>
struct RatioPow<R, 0, true>
{
	using type = Ratio<>;
};

} // namespace nd::math::units::detail

template <typename R, std::intmax_t Power>
using RatioPow = typename detail::RatioPow<R, Power, common::isPositive(Power)>::type;

template <typename RatioType>
using RatioNormalize = typename detail::RatioNormalize<RatioType::numerator, RatioType::denominator>::type;

using Atto  = Ratio<                        1u, 1'000'000'000'000'000'000u>;
using Femto = Ratio<                        1u,     1'000'000'000'000'000u>;
using Pico  = Ratio<                        1u,         1'000'000'000'000u>;
using Nano  = Ratio<                        1u,             1'000'000'000u>;
using Micro = Ratio<                        1u,                 1'000'000u>;
using Milli = Ratio<                        1u,                     1'000u>;
using Centi = Ratio<                        1u,                       100u>;
using Deci  = Ratio<                        1u,                        10u>;
using Deca  = Ratio<                        1u,                         1u>;
using Hecto = Ratio<                      100u,                         1u>;
using Kilo  = Ratio<                    1'000u,                         1u>;
using Mega  = Ratio<                1'000'000u,                         1u>;
using Giga  = Ratio<            1'000'000'000u,                         1u>;
using Tera  = Ratio<        1'000'000'000'000u,                         1u>;
using Peta  = Ratio<    1'000'000'000'000'000u,                         1u>;
using Exa   = Ratio<1'000'000'000'000'000'000u,                         1u>;

using Kibi  = Ratio<                    1'024u,                         1u>;
using Mebi  = Ratio<                1'048'576u,                         1u>;
using Gibi  = Ratio<            1'073'741'824u,                         1u>;
using Tebi  = Ratio<        1'099'511'627'776u,                         1u>;
using Pebi  = Ratio<    1'125'899'906'842'624u,                         1u>;
using Exbi  = Ratio<1'152'921'504'606'846'976u,                         1u>;

} // namespace nd::math::units

#endif // ND_MATH_UNITS_RATIO_HPP
