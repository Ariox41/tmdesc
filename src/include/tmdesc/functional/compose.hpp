// Copyright Victor Smirnov 2021
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
//
// The documentation can be found at the library's page:
// https://github.com/Ariox41/tmdesc

#pragma once
#include "construct.hpp"
#include "invoke.hpp"
#include "variadic_fold_left.hpp"
namespace tmdesc {
namespace detail {
template <class F1, class F2> struct composed {
    F1 f1_;
    F2 f2_;

    template <class A1, class... Args> constexpr operator()(A1&& a1, Args&&... args) const {
        return invoke(f1_, invoke(f2_, std::forward<A1>(a1)), std::forward<Args>(args)...);
    }
    template <class A1, class... Args> constexpr operator()(A1&& a1, Args&&... args) & {
        return invoke(f1_, invoke(f2_, std::forward<A1>(a1)), std::forward<Args>(args)...);
    }
    template <class A1, class... Args> constexpr operator()(A1&& a1, Args&&... args) && {
        return invoke(std::move(f1_), invoke(std::move(f2_), std::forward<A1>(a1)), std::forward<Args>(args)...);
    }
};

struct make_composed_t {
    template <class Fn> constexpr Fn&& operator()(Fn&& fn) noexcept { return fn; }
    template <class F1, class F2>
    constexpr composed<std::decay_t<F1>, std::decay_t<F2>> operator()(F1&& f1, F2&& f2) noexcept {
        return {std::forward<F1>(f1), std::forward<F2>(f2)};
    }
};

constexpr make_composed_t make_composed{};

} // namespace detail
struct compose_t {
    template <class F1, class F2, class... Fs> auto operator()(F1&& f1, Fs&&... fs) {
        return variadic_fold_left(detail::make_composed, std::forward<F1>(f1), std::forward<Fs>(fs)...);
    }
};

/// compose(f) => f
/// compose(f, g)(x, xs...) => f(g(x), xs...)
/// compose(f, compose(g, h))(xs...) == compose(compose(f, g), h)(xs...)
constexpr compose_t compose{};
} // namespace tmdesc
