// Copyright Victor Smirnov 2021
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
//
// The documentation can be found at the library's page:
// https://github.com/Ariox41/tmdesc
#pragma once
#include "../core/implementable_function.hpp"
#include "../meta/logical_operations.hpp"
#include "../meta/tag_of.hpp"

namespace tmdesc {

/// `eval_if` implementation for Logical
template <class T, class Enable = void> struct eval_if_impl : core::unimplemented {
    /// eval_if(cond, then, else_) => cond ? then(id) : else(id)
    template <class C, class T, class E> static constexpr auto apply(C&& cond, T&& then_, E&& else_) = delete;
};

/// `negate` implementation for Logical
template <class T, class Enable = void> struct negate_impl : core::unimplemented {
    /// eval_if(cond, then, else_) = cond ? then(id) : else(id)
    template <class I, class V> static constexpr auto apply(V&& v) = delete;
};

/// Logical types must implement the `eval_if` and `negate`.
template <class T>
struct Logical
  : meta::fast_values_and<core::has_implementation<eval_if_impl<T>>, core::has_implementation<negate_impl<T>>> {};

struct eval_if_t {
    template <class T> using impl_t = eval_if_impl<meta::tag_of_t<T>>;

    template <class C, class T, class E, std::enable_if_t<Logical<typename meta::tag_of<C>::type>{}, bool> = true>
    constexpr auto operator()(C&& cond, T&& then_, E&& else_) const
        noexcept(noexcept(impl_t<C>::apply(std::declval<C>(), std::declval<T>(), std::declval<E>())))
            -> decltype(impl_t<C>::apply(std::declval<C>(), std::declval<T>(), std::declval<E>())) {
        return impl_t<C>::apply(std::forward<C>(cond), std::forward<T>(then_), std::forward<E>(else_));
    };
};

/// negate(cond) => cond ? eval(then_) : eval(else_)
constexpr eval_if_t eval_if{};

struct negate_t {
    template <class T> using impl_t = negate_impl<meta::tag_of<T>>;

    template <class C, std::enable_if_t<Logical<typename meta::tag_of<C>::type>{}, bool> = true>
    constexpr auto operator()(C&& cond) const                   //
        noexcept(noexcept(impl_t<C>::apply(std::declval<C>()))) //
        -> decltype(impl_t<C>::apply(std::declval<C>())) {
        return impl_t<C>::apply(std::forward<C>(cond));
    }
};

/// negate(negate(cond)) => cond
constexpr negate_t negate{};

} // namespace tmdesc
