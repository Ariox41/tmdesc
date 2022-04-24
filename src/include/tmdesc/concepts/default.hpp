// Copyright Victor Smirnov 2021
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
//
// The documentation can be found at the library's page:
// https://github.com/Ariox41/tmdesc

#pragma once
#include "../meta/integral_constant.hpp"
#include "../meta/void_t.hpp"
namespace tmdesc {
/// Base class for marking an implementation as the default implementation
struct _default {};

template <class T, class = void> struct is_default : false_type {};

template <class T>
struct is_default<T, meta::void_t<decltype(static_cast<const _default&>(std::declval<const T&>()))>> : true_type {};
} // namespace tmdesc