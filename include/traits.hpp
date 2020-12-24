#ifndef ND_MATH_TRAITS_HPP
#define ND_MATH_TRAITS_HPP

#include <cstddef>
#include <cstdint>
#include <type_traits>

namespace nd::math::traits
{

namespace initialization
{

struct Zero {};
struct Identity {};
struct Radians {};
struct Degrees {};

inline constexpr Zero		zero		= Zero{};
inline constexpr Identity	identity	= Identity{};
inline constexpr Radians	radians		= Radians{};
inline constexpr Degrees	degrees		= Degrees{};

}

template <std::size_t N>
struct IsOne : std::false_type {};

template <>
struct IsOne<1u> : std::true_type {};

template <std::size_t N, std::size_t M>
struct IsEqualSize : std::false_type {};

template <std::size_t N>
struct IsEqualSize<N, N> : std::true_type {};

template <std::size_t N, std::size_t M, typename Unused_>
using EnableEqualSize = std::enable_if_t<IsEqualSize<N, M>::value, Unused_>;

template <std::uintmax_t N, std::uintmax_t M>
struct IsEqualUIntMax : std::false_type {};

template <std::uintmax_t N>
struct IsEqualUIntMax<N, N> : std::true_type {};

template <std::uintmax_t N, std::uintmax_t M, typename Unused_>
using EnableEqualUIntMax = std::enable_if_t<IsEqualUIntMax<N, M>::value, Unused_>;

template <std::uintmax_t N, std::uintmax_t M>
struct IsEqualIntMax : std::false_type {};

template <std::uintmax_t N>
struct IsEqualIntMax<N, N> : std::true_type {};

template <std::uintmax_t N, std::uintmax_t M, typename Unused_>
using EnableEqualIntMax = std::enable_if_t<IsEqualIntMax<N, M>::value, Unused_>;

template <typename ValueType>
struct HasNegative : std::disjunction<std::is_signed<ValueType>, std::is_floating_point<ValueType>> {};

template <typename ValueType, typename Unused_>
using EnableNegative = std::enable_if_t<HasNegative<ValueType>::value, Unused_>;

template <std::size_t Rows, std::size_t Columns>
struct IsScalar : std::conjunction<IsOne<Rows>, IsOne<Columns>> {};

template <std::size_t Rows, std::size_t Columns, typename Unused_>
using EnableScalar = std::enable_if_t<IsScalar<Rows, Columns>::value, Unused_>;

template <std::size_t Order>
struct Is2D : std::false_type {};

template <>
struct Is2D<2u> : std::true_type {};

template <std::size_t Order>
struct Is3D : std::false_type {};

template <>
struct Is3D<3u> : std::true_type {};

template <std::size_t Rows, std::size_t Columns, typename Unused_>
using Enable2DAnd3D = std::enable_if_t<std::conjunction_v<IsEqualSize<Rows, Columns>, std::disjunction<Is2D<Rows>, Is3D<Rows>>>, Unused_>;

template <std::size_t Rows, std::size_t Columns>
struct IsVector : std::disjunction<IsOne<Rows>, IsOne<Columns>> {};

template <std::size_t Rows, std::size_t Columns, typename Unused_>
using EnableVector = std::enable_if_t<IsVector<Rows, Columns>::value, Unused_>;

template <std::size_t Rows, std::size_t Columns>
struct Is3DVector : std::conjunction<std::disjunction<Is3D<Rows>, Is3D<Columns>>, IsVector<Rows, Columns>> {};

template <std::size_t Rows, std::size_t Columns, typename Unused_>
using Enable3DVector = std::enable_if_t<Is3DVector<Rows, Columns>::value, Unused_>;

template <std::size_t Rows, std::size_t Columns, std::size_t VectorSize, typename Unused_>
using EnableVectorSize = std::enable_if_t<
							std::conjunction_v<
								IsVector<Rows, Columns>,
								std::disjunction<IsEqualSize<VectorSize, Rows>, IsEqualSize<VectorSize, Columns>>>,
							Unused_>;

template <typename ValueType>
struct IsPod : std::disjunction<std::is_trivial<ValueType>, std::is_standard_layout<ValueType>> {};

template <typename ValueType>
using EnablePod = std::enable_if_t<IsPod<ValueType>::value, ValueType>;

template <std::size_t Rows, std::size_t Columns, typename Unused_>
using EnableNonZeroSizeVector = std::enable_if_t<((Rows * Columns) > 0u), Unused_>;

}

#endif // ND_MATH_TRAITS_HPP
