// Copyright Victor Smirnov 2021
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
//
// The documentation can be found at the library's page:
// https://github.com/Ariox41/tmdesc

#pragma once
#include "../core/integral_constant.hpp"
#include <initializer_list>

namespace tmdesc {
namespace meta {
/// recursive conjuction without argument precomputation
/// @see std::conjuction
template <class...> struct recursive_and : true_type {};
template <class B1> struct recursive_and<B1> : B1 {};
template <class B1, class... Bn>
struct recursive_and<B1, Bn...> : std::conditional_t<bool(B1::value), recursive_and<Bn...>, B1> {};
template <class... BS> constexpr bool recursive_and_v = recursive_and<BS...>::value;

/// recursive disjunction without argument precomputation
/// @see std::disjunction
template <class...> struct recursive_or : false_type {};
template <class B1> struct recursive_or<B1> : B1 {};
template <class B1, class... Bn>
struct recursive_or<B1, Bn...> : std::conditional_t<bool(B1::value), B1, recursive_or<Bn...>> {};
template <class... B> constexpr bool recursive_or_v = recursive_or<B...>::value;

template <class B> struct negation : std::integral_constant<bool, !bool(B::value)> {};
template <class B> constexpr bool negation_v = negation<B>::value;

/// Non-recursive conjunction.
/// \details Unlike @ref recursive_and, `fast_and` is not recursive, but the price is the precomputation of the argument values.
template <bool... BS> struct fast_and : std::is_same<fast_and<BS...>, fast_and<(BS, true)...>> {};
template <bool... BS> constexpr bool fast_and_v = fast_and<BS...>::value;

/// Non-recursive disjunction.
/// Unlike @ref disjunction, `fast_or` is not recursive, but the price is the precomputation of the argument values.
template <bool... BS> struct fast_or : negation<std::is_same<fast_or<BS...>, fast_or<(BS, false)...>>> {};
template <bool... BS> constexpr bool fast_or_v = fast_or<BS...>::value;

/// Non-recursive conjunction of `Bs::value...`
template <class... Bs> struct fast_values_and : fast_and<bool(Bs::value)...> {};
template <class... Bs> constexpr bool fast_values_and_v = fast_values_and<Bs...>::value;

/// Non-recursive disjunction of `Bs::value...`
template <class... Bs> struct fast_values_or : fast_or<bool(Bs::value)...> {};
template <class... Bs> constexpr bool fast_values_or_v = fast_values_or<Bs...>::value;

} // namespace meta
} // namespace tmdesc
