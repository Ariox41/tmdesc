// Copyright Victor Smirnov 2021
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
//
// The documentation can be found at the library's page:
// https://github.com/Ariox41/tmdesc

#pragma once
#include "ref_obj_fwd.hpp"
#include <functional>
namespace tmdesc {

template <class T> constexpr ref_obj<T> ref(T& v) noexcept { return {v}; }
template <class T> constexpr ref_obj<T> ref(ref_obj<T> v) noexcept { return v; }
template <class T> constexpr void ref(const T&& v) = delete;

template <class T> constexpr ref_obj<const T> cref(T& v) noexcept { return {v}; }
template <class T> constexpr ref_obj<const T> cref(ref_obj<T> v) noexcept { return v; }
template <class T> constexpr void cref(const T&& v) = delete;

template <class To> struct ref_cast_fn {
    template <class From, std::enable_if_t<std::is_convertible<From&, To&>::value, bool> = true>
    constexpr ref_obj<To> operator()(ref_obj<From> from) const noexcept {
        return ref_obj<To>{from.get()};
    }
    template <class From, std::enable_if_t<std::is_convertible<From&, To&>::value, bool> = true>
    constexpr ref_obj<const To> operator()(ref_obj<const From> from) const noexcept {
        return ref_obj<const To>{from.get()};
    }
};

/// Functional object for explicit cast `ref_obj<From>` to `ref_obj<To>`
/// @note ref_obj<const From> cast to ref_obj<const To>
template <class To> constexpr ref_cast_fn<To> ref_cast{};

struct inderect_ref_fn {
    template <class T>
    constexpr auto operator()(T& ptr) const noexcept(noexcept(*std::declval<T&>()))
        -> ref_obj<std::remove_reference_t<decltype(*std::declval<T&>())>> {
        return {*ptr};
    }
    template <class T>
    constexpr auto operator()(ref_obj<T> ref) const noexcept(noexcept(*std::declval<T&>()))
        -> ref_obj<std::remove_reference_t<decltype(*std::declval<T&>())>> {
        return {*ref.get()};
    }
};

/**
 * @brief Functional object for explicit dereference pointer-like object  to `ref_obj`
 * @details
 * - Converts Pointer to ref_obj<decltype(*declval<Pointer>())>
 * - Converts ref_obj<Pointer> to ref_obj<decltype(*declval<Pointer>())>
 */
constexpr inderect_ref_fn inderect_ref{};

} // namespace tmdesc
