// Copyright Victor Smirnov 2021
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
//
// The documentation can be found at the library's page:
// https://github.com/Ariox41/tmdesc

#pragma once
#include "../core/implementable_function.hpp"
#include "fake_argument.hpp"

namespace tmdesc {

template <class T, class Enable = void> struct eval_impl : core::default_implementation {
    template <class Fn>
    static constexpr auto apply(Fn&& fn)                                      //
        noexcept(noexcept(eval_impl::apply_helper(std::declval<Fn>(), true))) //
        -> decltype(eval_impl::apply_helper(std::declval<Fn>(), true)) {
        return eval_impl::apply_helper(std::forward<Fn>(fn), true);
    }

    template <class Fn>
    static constexpr auto apply_helper(Fn&& fn, bool) noexcept(noexcept(std::declval<Fn>()()))
        -> decltype(std::declval<Fn>()()) {
        return std::forward<Fn>(fn)();
    }
    template <class Fn>
    static constexpr auto apply_helper(Fn&& fn, int) noexcept(noexcept(std::declval<Fn>()(fake_argument)))
        -> decltype(std::declval<Fn>()(fake_argument)) {
        return std::forward<Fn>(fn)(fake_argument);
    }
};

struct eval_t {
    template <class T> using impl_t = eval_impl<std::decay_t<T>>;

    template <class Fn>
    constexpr auto operator()(Fn&& fn) const                      //
        noexcept(noexcept(impl_t<Fn>::apply(std::declval<Fn>()))) //
        -> decltype(impl_t<Fn>::apply(std::declval<Fn>())) {
        return impl_t<Fn>::apply(std::forward<Fn>(fn));
    }
};

/// Evalute function if it is has no argument, o one unused generic argument, of if it is a complete `lazy_fn`.
///
/// eval([]{}) => invoke([]{})
/// eval([](auto){}) => invoke([](auto){}, fake_argument)
/// eval(lazy_fn([](custom_argument){}, make_custom_argument())) => invoke([](custom_argument){}, make_custom_argument())
constexpr eval_t eval{};

} // namespace tmdesc
