// Copyright Victor Smirnov 2021-2023
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
//
// The documentation can be found at the library's page:
// https://github.com/Ariox41/tmdesc

#pragma once

#include <type_traits> // for forward declaration of reference_wrapper and for decay_t
namespace tmdesc {
/**
 * @brief Represents reference as object with address and copy constructor/assignment.
 * @details It is an `std::reference_wrapper` alias with helpers.
 * @note The `std::reference_wrapper` does not support implicit reference conversions
 * - no `ref<T>` to `ref<const T>`
 * - no `ref<Derived>` to `ref<Base>`
 * @note `std::reference_wrapper` does not have a constexpr constructor before c++20
 * @see ref_cast for explicit conversions
 * @see pointer_to_ref for explicit pointer dereference
 */
template <class T> using ref_obj = std::reference_wrapper<T>;

template <class T> struct is_ref_obj : std::false_type {};
template <class U> struct is_ref_obj<ref_obj<U>> : std::true_type {};
template <class T> constexpr bool is_ref_obj_v = is_ref_obj<T>::value;

/// If T is `ref_obj`, then provides the `ref_obj` raw reference type. Otherwise, provides original T.
template <class T> struct try_unwrap_ref_obj_type { using type = T; };
template <class T> struct try_unwrap_ref_obj_type<ref_obj<T>> { using type = T&; };
template <class T> using try_unwrap_ref_obj_type_t = typename try_unwrap_ref_obj_type<T>::type;
} // namespace tmdesc
