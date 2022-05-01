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

/// `unpack` implementation interface for foldable type
template <class T, class Enable = void> struct unpack_impl : core::unimplemented {
    /// v = [v1, v2, ..., vN] => fn(v1, v2, ..., vN)
    template <class V, class Fn> static constexpr auto apply(V&& v, Fn&& fn) = delete;
};
/// Foldable types must implement the `unpack_impl`.
/// \todo Are there any cases when it is easier to define `fold_left` and use it to implement `unpack`?
template <class T> struct Foldable : core::has_implementation<unpack_impl<T>> {};

struct unpack_t {
    template <class T> using impl_t = unpack_impl<meta::tag_of_t<T>>;

    template <class T, class Fn, std::enable_if_t<Foldable<typename meta::tag_of<T>::type>{}, bool> = true>
    constexpr auto operator()(T&& v, Fn&& fn) const //
        noexcept(noexcept(impl_t<T>::apply(std::declval<T>(), std::declval<Fn>())))
            -> decltype(impl_t<T>::apply(std::declval<T>(), std::declval<Fn>())) {
        return impl_t<T>::apply(std::forward<T>(v), std::forward<Fn>(fn));
    }
};

/// unpack(v = [v1, v2, ..., vN], fn) => fn(v1, v2, ..., vn)
constexpr unpack_t unpack{};

} // namespace tmdesc
