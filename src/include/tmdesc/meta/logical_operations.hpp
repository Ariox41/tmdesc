// Copyright Victor Smirnov 2021
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
//
// The documentation can be found at the library's page:
// https://github.com/Ariox41/tmdesc

#pragma once
#include <initializer_list>
#include <type_traits>

namespace tmdesc {
namespace meta {

///\deprecated bool_and has same perfomance and it has better interface
inline constexpr bool fast_and(std::initializer_list<bool> list) noexcept {
    bool ret = true;
    for (bool v : list) {
        ret = ret && v;
    }
    return ret;
}
///\deprecated bool_and has same perfomance and it has better interface
inline constexpr bool fast_or(std::initializer_list<bool> list) noexcept {
    bool ret = false;
    for (bool v : list) {
        ret = ret || v;
    }
    return ret;
}

/// recursive conjunction without argument precomputation
/// @see std::conjunction
template <class...> struct conjunction : std::true_type {};
template <class B1> struct conjunction<B1> : B1 {};
template <class B1, class... Bn>
struct conjunction<B1, Bn...> : std::conditional_t<bool(B1::value), conjunction<Bn...>, B1> {};
template <class... Bools> constexpr bool conjunction_v = conjunction<Bools...>::value;

/// recursive disjunction without argument precomputation
/// @see std::disjunction
template <class...> struct disjunction : std::false_type {};
template <class B1> struct disjunction<B1> : B1 {};
template <class B1, class... Bn>
struct disjunction<B1, Bn...> : std::conditional_t<bool(B1::value), B1, disjunction<Bn...>> {};
template <class... B> constexpr bool disjunction_v = disjunction<B...>::value;

template <class B> struct negation : std::integral_constant<bool, !bool(B::value)> {};
template <class B> constexpr bool negation_v = negation<B>::value;

/// Unlike @ref conjunction, `bool_and` is not recursive, but the price is the precomputation of the argument values.
template <bool... Bools> struct bool_and : std::is_same<bool_and<Bools...>, bool_and<(Bools, true)...>> {};
template <bool... Bools> constexpr bool bool_and_v = bool_and<Bools...>::value;

/// Unlike @ref disjunction, `bool_or` is not recursive, but the price is the precomputation of the argument values.
template <bool... Bools> struct bool_or : negation<std::is_same<bool_or<Bools...>, bool_or<(Bools, false)...>>> {};
template <bool... Bools> constexpr bool bool_or_v = bool_or<Bools...>::value;

template <class... Bs> struct bool_type_and : bool_and<bool(Bs::value)...> {};
template <class... Bs> constexpr bool bool_type_and_v = bool_type_and<Bs...>::value;

template <class... Bs> struct bool_type_or : bool_or<bool(Bs::value)...> {};
template <class... Bs> constexpr bool bool_type_or_v = bool_type_or<Bs...>::value;

} // namespace meta
} // namespace tmdesc
