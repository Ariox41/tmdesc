// Copyright Victor Smirnov 2021
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
//
// The documentation can be found at the library's page:
// https://github.com/Ariox41/tmdesc

#pragma once
#include "../functional/invoke.hpp"
#include "../meta/logical_operations.hpp"
#include "foldable.hpp"
#include "indexable.hpp"

namespace tmdesc {

/// `size` implementation for indexable type
template <class T, class Enable = void> struct size_impl : core::unimplemented {
    /// v = [v0, v1, ..., vN] => size_c<N + 1>
    template <class V> static constexpr auto apply(V&& v) = delete;
};

/// Indexable types must implement the `at_impl` and `size_impl`.
template <class T>
struct FiniteIndexable : meta::recursive_and<core::has_implementation<size_impl<T>>, Indexable<T>> {};

struct size_of_t {
    template <class T> using impl_t = size_impl<meta::tag_of_t<T>>;

    template <class V, std::enable_if_t<FiniteIndexable<typename meta::tag_of<V>::type>{}, bool> = true>
    constexpr auto operator()(V&& v) const                      //
        noexcept(noexcept(impl_t<V>::apply(std::declval<V>()))) //
        -> decltype(impl_t<V>::apply(std::declval<V>())) {
        return impl_t<V>::apply(std::forward<V>(v));
    }
};

/// v = [v1, v2, ..., vN] => v [index]
constexpr size_of_t size{};

// ========== FinitIndexable => Foldable  ==============
template <class T> struct unpack_impl<T, std::enable_if_t<FiniteIndexable<T>{}>> {
    template <class V, class Fn, std::size_t... I>
    static constexpr auto apply_impl(V&& v, Fn&& fn, index_sequence<I...>) //
        noexcept(noexcept(invoke(std::declval<Fn>(), at(size_c<I>, std::declval<V>())...)))
            -> decltype(invoke(std::declval<Fn>(), at(size_c<I>, std::declval<V>())...)) {
        return invoke(std::forward<Fn>(fn), at(size_c<I>, std::forward<V>(v))...);
    }

    template <class V, class Fn>
    static constexpr auto apply(V&& v, Fn&& fn) //
        noexcept(noexcept(apply_impl(std::declval<V>(), std::declval<Fn>(),
                                     index_sequence_up_to(size(std::declval<V>())))))
            -> decltype(apply_impl(std::declval<V>(), std::declval<Fn>(),
                                   index_sequence_up_to(size(std::declval<V>())))) {
        return apply_impl(std::forward<V>(v), std::forward<Fn>(fn), index_sequence_up_to(size(std::forward<V>(v))));
    }
};

} // namespace tmdesc
