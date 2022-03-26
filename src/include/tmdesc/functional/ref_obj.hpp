// Copyright Victor Smirnov 2021
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
//
// The documentation can be found at the library's page:
// https://github.com/Ariox41/tmdesc

#pragma once
// for forward declaration of reference_wrapper and for decay_t
#include <type_traits>

namespace tmdesc {

/** Represents reference as object with address and copy constructor/assignment.
    @details
        It is an `std::reference_wrapper` alias, but it may change in the future.
        The `std::reference_wrapper` does not support implicit reference conversions
        - no `ref<T>` to `ref<const T>`
        - no `ref<Derived>` to `ref<Base>`
    @note #include <functional> requared for  `std::reference_wrapper`
    @note `std::reference_wrapper` does not have a constexpr constructor before c++20
        
    @see ref_cast for explicit conversions
    @see pointer_to_ref for explicit pointer dereference
*/
template <class T> using ref_obj = std::reference_wrapper<T>;

template <class T> struct is_ref_obj : std::false_type {};
template <class U> struct is_ref_obj<ref_obj<U>> : std::true_type {};
template <class T> constexpr bool is_ref_obj_v = is_ref_obj<T>::value;

/// If T is ref_obj, then provides the ref_obj raw reference type. Otherwise, provides original T.
template <class T> struct try_unwrap_ref_obj_type { using type = T; };
template <class T> struct try_unwrap_ref_obj_type<ref_obj<T>> { using type = T&; };
template <class T> using try_unwrap_ref_obj_type_t = typename try_unwrap_ref_obj_type<T>::type;

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

struct pointer_to_ref_fn {
    template <class T>
    constexpr auto operator()(T& ptr) const noexcept(noexcept(*std::declval<T&>()))
        -> ref_obj<std::remove_reference_t<decltype(*std::decay_t<T&>())>> {
        return {*ptr};
    }
    template <class T>
    constexpr auto operator()(ref_obj<T> ref) const noexcept(noexcept(*std::declval<T&>()))
        -> ref_obj<std::remove_reference_t<decltype(*std::decay_t<T&>())>> {
        return {*ref.get()};
    }
};

/// Functional object for explicit pointer-like object dereference to `ref_obj`
/// @note `ref_obj<Pointer>` is converted to `ref_obj<deref<Pointer>>`
/// @note Conversion can generate a dangling link if used carelessly
constexpr pointer_to_ref_fn pointer_to_ref{};

} // namespace tmdesc
