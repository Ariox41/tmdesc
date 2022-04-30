// Copyright Victor Smirnov 2021
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
//
// The documentation can be found at the library's page:
// https://github.com/Ariox41/tmdesc
#pragma once
#include "../meta/implementable_function.hpp"
#include "optional.hpp"

namespace tmdesc {

/// `find_if` implementation for searchable type
template <class T, class Enable = void> struct find_if_impl : meta::unimplemented {
    /// Finds a value whose key satisfies the predicate and returns some(value) or none
    template <class C, class P> static constexpr auto apply(C&& container, P&& predicate);
};

/// Searchable types must implement the `find_if_impl`.
template <class T> struct is_searchable : meta::has_implementation<find_if_impl<T>> {};

struct find_if_t {
    template <class T> using impl_t = find_if_impl<std::decay_t<T>>;

    template <class T, class P, std::enable_if_t<is_searchable<std::decay_t<T>>::value, bool> = true>
    constexpr auto operator()(T&& v, P&& pred) const //
        noexcept(noexcept(impl_t<T>::apply(std::declval<T>(), std::declval<P>())))
            -> decltype(impl_t<T>::apply(std::declval<T>(), std::declval<P>())) {
        return impl_t<T>::apply(std::forward<T>(v), std::forward<P>(pred));
    }
};

/// Finds a value whose key satisfies the predicate and returns some(value) or none
constexpr find_if_t find_if{};
} // namespace tmdesc