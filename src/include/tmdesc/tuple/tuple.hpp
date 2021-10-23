// Copyright Victor Smirnov 2021
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
//
// The documentation can be found at the library's page:
// https://github.com/Ariox41/tmdesc

#pragma once

#include "../functional/reference_wrapper_helpers.hpp"
#include "../get.hpp"
#include "detail/tuple.hpp"
#include <utility>

namespace tmdesc {
/// full constexpr inherit-based tuple without recursive implementation
template <class... Ts>
struct tuple : public detail::tuple_impl<std::make_index_sequence<sizeof...(Ts)>, Ts...> {
    constexpr tuple()             = default;
    constexpr tuple(const tuple&) = default;
    constexpr tuple(tuple&&)      = default;
    constexpr tuple& operator=(const tuple&) = default;
    constexpr tuple& operator=(tuple&&) = default;

    using detail::tuple_impl<std::make_index_sequence<sizeof...(Ts)>, Ts...>::tuple_impl;
};

template <class T> struct is_tuple : std::false_type {};
template <class... Ts> struct is_tuple<tuple<Ts...>> : std::true_type {};
template <class T> constexpr bool is_tuple_v = is_tuple<T>::value;

template <class... Ts>
struct tuple_size<tuple<Ts...>, std::enable_if_t<true>>
  : public std::integral_constant<std::size_t, sizeof...(Ts)> {};

template <class... Ts>
constexpr tuple<unwrap_ref_decay_t<Ts>...> make_tuple(Ts&&... ts) noexcept(
    meta::conjunction_v<std::is_nothrow_constructible<unwrap_ref_decay_t<Ts>, Ts>...>) {
    return tuple<unwrap_ref_decay_t<Ts>...>{std::forward<Ts>(ts)...};
}

template <class... Ts> constexpr tuple<Ts&...> tie(Ts&... ts) noexcept { return {ts...}; }

template <class... Ts> struct tuple_getter<tuple<Ts...>> {
    using type = detail::getter_for_tuple_t;
};
template <class... Ts> struct tuple_getter_by_type<tuple<Ts...>> {
    using type = detail::getter_by_type_for_tuple_t;
};

// template <std::size_t I, class Tuple, std::enable_if_t<is_tuple_v<meta::remove_cvref_t<Tuple>>,
// bool> = true> constexpr decltype(auto) get(Tuple&& t) noexcept {
//    return detail::getter_for_tuple_t::template get_by_id<I>(std::forward<Tuple>(t));
//}
// template <class T, class Tuple, std::enable_if_t<is_tuple_v<meta::remove_cvref_t<Tuple>>, bool> =
// true> constexpr decltype(auto) get(Tuple&& t) noexcept {
//    return detail::getter_for_tuple_t::template get_by_type<T>(std::forward<Tuple>(t));
//}

} // namespace tmdesc
