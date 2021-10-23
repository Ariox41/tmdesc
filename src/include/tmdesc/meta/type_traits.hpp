// Copyright Victor Smirnov 2021
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
//
// The documentation can be found at the library's page:
// https://github.com/Ariox41/tmdesc

#pragma once
#include <type_traits>
namespace tmdesc {
namespace meta {

template <class T> struct remove_cvref : public std::remove_cv<std::remove_reference_t<T>> {};
template <class T> using remove_cvref_t = typename remove_cvref<T>::type;

using std::is_constructible;
template <class T, class... Args>
constexpr bool is_constructible_v = is_constructible<T, Args...>::value;

using std::is_nothrow_constructible;
template <class T, class... Args>
constexpr bool is_nothrow_constructible_v = is_nothrow_constructible<T, Args...>::value;

} // namespace meta
} // namespace tmdesc
