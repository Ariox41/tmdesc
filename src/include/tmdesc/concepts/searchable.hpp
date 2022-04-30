// Copyright Victor Smirnov 2021
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
//
// The documentation can be found at the library's page:
// https://github.com/Ariox41/tmdesc
#pragma once
#include "../containers/optional.hpp"
#include "../core/implementable_function.hpp"
#include "../meta/tag_of.hpp"

namespace tmdesc {

/// `find_if` implementation for searchable type
template <class T, class Enable = void> struct find_if_impl : core::unimplemented {
    /// Finds a value whose key satisfies the predicate and returns some(value) or none
    template <class C, class P> static constexpr auto apply(C&& container, P&& predicate) = delete;
};

/// Searchable types must implement the `find_if_impl`.
template <class T> struct Searchable : core::has_implementation<find_if_impl<T>> {};

struct find_if_t {
    template <class T> using impl_t = find_if_impl<meta::tag_of_t<T>>;

    template <class T, class P, std::enable_if_t<Searchable<typename meta::tag_of<T>::type>{}, bool> = true>
    constexpr auto operator()(T&& v, P&& pred) const //
        noexcept(noexcept(impl_t<T>::apply(std::declval<T>(), std::declval<P>())))
            -> decltype(impl_t<T>::apply(std::declval<T>(), std::declval<P>())) {
        return impl_t<T>::apply(std::forward<T>(v), std::forward<P>(pred));
    }
};

/// Finds a value whose key satisfies the predicate and returns some(value) or none
constexpr find_if_t find_if{};
} // namespace tmdesc
