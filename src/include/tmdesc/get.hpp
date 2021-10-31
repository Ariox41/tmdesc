// Copyright Victor Smirnov 2021
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
//
// The documentation can be found at the library's page:
// https://github.com/Ariox41/tmdesc

#pragma once
#include "meta/type_traits.hpp"
#include "meta/void_t.hpp"
#include <utility>
namespace tmdesc {

/// Tuple size customisation point
template <class T, class = void> struct tuple_size;

/// tuple_size of const type equal to tuple_size of original type, but tuple_size of reference is
/// undefined
/// @note specializations are allowed only for nonconstant types
template <class T>
struct tuple_size<T, std::enable_if_t<std::is_const<T>::value>>
  : public tuple_size<std::remove_const_t<T>> {};

template <class Tuple> constexpr std::size_t tuple_size_v = tuple_size<Tuple, void>::value;

/// make index sequence for remove_const_t<remove_reference_t<Tuple>>
template <class Tuple>
using index_sequence_for_tuple =
    std::make_index_sequence<tuple_size_v<std::remove_reference_t<Tuple>>>;

/** `get` by index customisation point.

    The implementation type must provide a `type` type member,
    that provide a static `get_by_id<I>(tuple)` method and
    `template<std::size_t I, class Tuple> using tuple_element=...` type member
*/
template <class Tuple, class = meta::void_t<>> struct tuple_getter;
template <class T> using tuple_getter_t = typename tuple_getter<T>::type;

/// `get` by type customisation point.
/// @details The implementation type must provide a static `get_by_type<T>(tuple)` method
template <class Tuple, class = meta::void_t<>> struct tuple_getter_by_type;
template <class T> using tuple_getter_by_type_t = typename tuple_getter_by_type<T>::type;

/// @return The `I`-th element of the tuple-like object `t`
/// @tparam I - index of element
template <std::size_t I, class Tuple> constexpr decltype(auto) get(Tuple&& t) noexcept {
    return tuple_getter_t<std::decay_t<Tuple>>::template get_by_id<I>(
                                                  static_cast<Tuple&&>(t));
}

/// @return Fine element with type T in the tuple-like object `t`
/// @tparam T - exact type of element
/// @note In general, the type `T` can be a tag, not only an element type
template <class T, class Tuple> constexpr decltype(auto) get(Tuple&& t) noexcept {
    return tuple_getter_by_type_t<std::decay_t<Tuple>>::template get_by_type<T>(
        std::forward<Tuple>(t));
}

/// The exact type of the `Tuple` element with the index `I`
template <std::size_t I, class Tuple>
struct tuple_element : public tuple_getter_t<Tuple>::template tuple_element<I, Tuple> {};

/// The type of `get<I>(declval<Tuple>())`
/// @details result type of get depends from `Tuple` qualifiers
template <std::size_t I, class Tuple>
using tuple_get_result_t = decltype(get<I>(std::declval<Tuple>()));

template <std::size_t I, class Tuple>
using tuple_element_t = typename tuple_element<I, Tuple>::type;
} // namespace tmdesc
