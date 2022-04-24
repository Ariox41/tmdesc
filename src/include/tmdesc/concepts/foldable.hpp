// Copyright Victor Smirnov 2021
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
//
// The documentation can be found at the library's page:
// https://github.com/Ariox41/tmdesc
#pragma once
#include "default.hpp"

namespace tmdesc {

/// `unpack` implementation for foldable type
template <class T, class Enable = meta::void_t<>> struct unpack_impl : _default {
    /// v = [v1, v2, ..., vN] => fn(v1, v2, ..., vN)
    template <class V, class Fn> static constexpr auto apply(V&& v, Fn&& fn);
};

template <class T> struct is_foldable : bool_constant<!is_default<unpack_impl<T>>{}> {};

struct unpack_t {
    template <class T> using impl_t = unpack_impl<std::decay_t<T>>;

    template <class T, class Fn, std::enable_if_t<is_foldable<std::decay_t<T>>::value, bool> = true>
    constexpr auto operator()(T&& v, Fn&& fn) const //
        noexcept(noexcept(impl_t<T>::apply(std::declval<T>(), std::declval<Fn>())))
            -> decltype(impl_t<T>::apply(std::declval<T>(), std::declval<Fn>())) {
        return impl_t<T>::apply(std::forward<T>(v), std::forward<Fn>(fn));
    }
};

/// unpack(v = [v1, v2, ..., vN], fn) => fn(v1, v2, ..., vn)
constexpr unpack_t unpack{};
} // namespace tmdesc
