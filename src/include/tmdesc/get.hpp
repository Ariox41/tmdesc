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

/** Provides access to the number of elements in a tuple-like type as a compile-time constant expression
@details `tuple_size` is a SFINAE-friendly customisation point

@tparam T
    A tuple-like type

@tparam AlwaysVoid
    The parameter for [SFINAE in partialspecializations](https://en.cppreference.com/w/cpp/language/sfinae#SFINAE_in_partial_specializations)
*/
template <class T, class AlwaysVoid = void> struct tuple_size;

/// Specializations for const-qualified types reuse the value
/// from the corresponding const-unqualified versions by default
template <class T> struct tuple_size<const T> : public tuple_size<T> {};

/// Helper variable template for @ref tuple_size
template <class Tuple> constexpr std::size_t tuple_size_v = tuple_size<Tuple, void>::value;

/// @return @ref tuple_size of a tuple-like object
template <class Tuple> constexpr std::size_t tuple_size_of(Tuple&&) noexcept {
    return tuple_size_v<std::decay_t<Tuple>>;
}

/** Make index sequence for `remove_reference_t<Tuple>`

    @tparam Tuple
    The tuple like type with the `tuple_size` specialisation
*/
template <class Tuple>
using index_sequence_for_tuple = std::make_index_sequence<tuple_size_v<std::remove_reference_t<Tuple>>>;

/** @ref get by index customisation point.

Specialisations must provide a `type` type member,
that provide `get_by_id` funtion and `tuple_element` type:
@code
    struct type{
        template<std::size_t I, class Tuple>
        static decltype(auto) get_by_id(Tuple&& t) noexcept;

        template<std::size_t I, class Tuple> using tuple_element = implementation;
    };
@endcode
*/
template <class Tuple, class = meta::void_t<>> struct tuple_getter;
template <class T> using tuple_getter_t = typename tuple_getter<T>::type;

/** @ref get by type customisation point.

Specialisations must provide a `type` type member that provide `get_by_type` funtion
@code
    struct type{
        template<class T, class Tuple>
        static decltype(auto) get_by_type(Tuple&& t) noexcept;
    };
@endcode
*/
template <class Tuple, class = meta::void_t<>> struct tuple_getter_by_type;
template <class T> using tuple_getter_by_type_t = typename tuple_getter_by_type<T>::type;

/// @return The `I`-th element of the tuple-like object `t`
/// @tparam I - index of element
template <std::size_t I, class Tuple> constexpr decltype(auto) get(Tuple&& t) noexcept {
    return tuple_getter_t<std::decay_t<Tuple>>::template get_by_id<I>(static_cast<Tuple&&>(t));
}

/// @return Find element of type `T` in the tuple-like object `t`
/// @tparam T - exact type of element
/// @note In general, the type `T` can be a tag, not only an element type
template <class T, class Tuple> constexpr decltype(auto) get(Tuple&& t) noexcept {
    return tuple_getter_by_type_t<std::decay_t<Tuple>>::template get_by_type<T>(std::forward<Tuple>(t));
}

/// The exact type of the `Tuple` element with the index `I`
template <std::size_t I, class Tuple>
struct tuple_element : public tuple_getter_t<Tuple>::template tuple_element<I, Tuple> {};
template <std::size_t I, class Tuple> using tuple_element_t = typename tuple_element<I, Tuple>::type;

/// The type of `get<I>(declval<Tuple>())`
/// @details result type of get depends from `Tuple` qualifiers
template <std::size_t I, class Tuple> using tuple_get_result_t = decltype(get<I>(std::declval<Tuple>()));

} // namespace tmdesc
