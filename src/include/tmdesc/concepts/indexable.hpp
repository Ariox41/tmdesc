// Copyright Victor Smirnov 2021
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
//
// The documentation can be found at the library's page:
// https://github.com/Ariox41/tmdesc
#pragma once
#include "../core/implementable_function.hpp"
#include "../meta/tag_of.hpp"

namespace tmdesc {

/// `at` implementation for indexable type
template <class T, class Enable = void> struct at_impl : core::unimplemented {
    /// v = [v0, v1, ..., vN] => v [index]
    template <class I, class V> static constexpr auto apply(I&& index, V&& v) = delete;
};

/// Indexable types must implement the `at_impl`.
template <class T> struct Indexable : core::has_implementation<at_impl<T>> {};

struct at_t {
    template <class T> using impl_t = at_impl<meta::tag_of_t<T>>;

    template <class I, class V, std::enable_if_t<Indexable<typename meta::tag_of<V>::type>{}, bool> = true>
    constexpr auto operator()(I&& i, V&& v) const //
        noexcept(noexcept(impl_t<V>::apply(std::declval<I>(), std::declval<V>())))
            -> decltype(impl_t<V>::apply(std::declval<I>(), std::declval<V>())) {
        return impl_t<V>::apply(std::forward<I>(i), std::forward<V>(v));
    }
};

/// v = [v1, v2, ..., vN] => v [index]
constexpr at_t at{};

template <std::size_t I, class T, std::enable_if_t<Indexable<typename meta::tag_of<T>::type>{}, bool> = true>
constexpr auto at_c(T&& v) noexcept(noexcept(at(size_c<I>, std::declval<T>())))
    -> decltype(at(size_c<I>, std::declval<T>())) {
    return at(size_c<I>, std::forward<T>(v));
}

} // namespace tmdesc
