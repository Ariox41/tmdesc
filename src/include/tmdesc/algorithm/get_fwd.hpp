// Copyright Victor Smirnov 2021
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)

#pragma once
#include "../meta/void_t.hpp"
#include "../meta/type_traits.hpp"
#include <utility>
namespace tmdesc {
/// Tuple size customisation point
template <class T, class = void> struct tuple_size;
template <class T>
struct tuple_size<T, std::enable_if_t<std::is_const<T>::value>> : public tuple_size<std::remove_const_t<T>> {};
template <class Tuple> constexpr std::size_t tuple_size_v = tuple_size<Tuple, void>::value;

template <class Tuple>
using index_sequence_for_tuple = std::make_index_sequence<tuple_size_v<std::remove_reference_t<Tuple>>>;

/// Tuple get customisation point
template <class Tuple, class = meta::void_t<>> struct tuple_getter;

template <class T> using tuple_getter_t = typename tuple_getter<T>::type;

template <std::size_t I, class Tuple> constexpr decltype(auto) get(Tuple&& t) noexcept {
    constexpr tuple_getter_t<meta::remove_cvref_t<Tuple>> geter;
    return geter.template get_by_id<I>(std::forward<Tuple>(t));
}
template <class T, class Tuple> constexpr decltype(auto) get(Tuple&& t) noexcept {
    return tuple_getter_t<meta::remove_cvref_t<Tuple>>::template get_by_type<T>(std::forward<Tuple>(t));
}

template <std::size_t I, class Tuple> using tuple_get_result_t = decltype(get<I>(std::declval<Tuple>()));

template <std::size_t I, class Tuple>
struct tuple_element : public tuple_getter_t<Tuple>::template tuple_element<I, Tuple>{};

template <std::size_t I, class Tuple> using tuple_element_t = typename tuple_element<I, Tuple>::type;
} // namespace tmdesc
