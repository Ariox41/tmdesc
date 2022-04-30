// Copyright Victor Smirnov 2021
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
//
// The documentation can be found at the library's page:
// https://github.com/Ariox41/tmdesc

#pragma once
#include "integral_constant.hpp"
#include "void_t.hpp"
namespace tmdesc {
namespace meta {

/// Base class for marking type as unimplemented function
struct unimplemented {};

/// Base class for marking type as default function implementation
struct default_implementation {};

template <class T, class = void> struct has_implementation : true_type {};

template <class T>
struct has_implementation<T, void_t<decltype(static_cast<const unimplemented&>(std::declval<const T&>()))>>
  : false_type {};

template <class T, class = void> struct is_default_implementation : false_type {};

template <class T>
struct has_implementation<T, void_t<decltype(static_cast<const default_implementation&>(std::declval<const T&>()))>>
  : true_type {};

} // namespace meta
} // namespace tmdesc