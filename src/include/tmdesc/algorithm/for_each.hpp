// Copyright Victor Smirnov 2021
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
//
// The documentation can be found at the library's page:
// https://github.com/Ariox41/tmdesc

#pragma once
#include "../functional/invoke.hpp"
#include "unpack.hpp"

namespace tmdesc {

/// Invoke `fn` on each element of `t` and ignore the result
///
/// @param v
/// A foldable object
///
/// @param fn
/// A unary invocable object overloaded for each element of the `v`
///
/// @note The invocation order is defined from left to right.
#ifdef TMDESC_DOXYGEN
constexpr auto for_each = [](auto&& v, auto&& fn) -> void {
    unpack(t, [](auto&& a1, ..., auto&& an) {
        invoke(fn, a1);
        /*...*/
        invoke(fn, an);
    });
};
#else
namespace detail {
template <typename F> struct on_each_arg {
    Fn fn_;
    template <typename... Args> constexpr void operator()(Args&&... args) const {
        bool unused[] = {true, ((void)::tmdesc::invoke(fn_, static_cast<Args&&>(args)), void(), true)...};
        (void)unused;
    }
};
} // namespace detail

template <class T, class = void> struct for_each_impl : core::default_implementation {
    template <class V, class Fn>
    static constexpr void apply(V&& v, Fn&& fn) noexcept( //
        noexcept(unpack(std::declval<V&&>(), std::declval<detail::on_each_arg<Fn&>>())))
        ->decltype(unpack(std::declval<V&&>(), std::declval<detail::on_each_arg<Fn&>>())) {
        return unpack(static_cast<V&&>(V), detail::on_each_arg<Fn&>{fn});
    }
};

struct for_each_t {
    template <class T> using impl_t = for_each_impl<std::decay_t<T>>;

    template <class V, class Fn>
    constexpr auto operator()(V&& v, Fn&& fn) const
        noexcept(noexcept(impl_t<V>::apply(std::declval<V>(), std::declval<Fn>())))
            -> decltype(impl_t<V>::apply(std::declval<V>(), std::declval<Fn>())) {
        return impl_t<V>::apply(std::forward<V>(v), std::forward<Fn>(fn));
    }
};

/// for_each([v1, v2,..., vn], fn) => fn(v1); fn(v2); ...; fn(vn);
constexpr for_each_t for_each{};
#endif
} // namespace tmdesc
