// Copyright Victor Smirnov 2021
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
//
// The documentation can be found at the library's page:
// https://github.com/Ariox41/tmdesc

#pragma once
#include "void_t.hpp"
#include <type_traits>
namespace tmdesc {
namespace meta {
/// Metafunction returning the tag associated to T.
/// The type tag defines the operations available for the type.
template <class T, class = void> struct tag_of;
template <class T> struct tag_of<T&> : tag_of<T> {};
template <class T> struct tag_of<T&&> : tag_of<T> {};
template <class T> struct tag_of<const T> : tag_of<T> {};
template <class T> struct tag_of<const T&> : tag_of<T> {};

template <class T> using tag_of_t = typename tag_of<T>::type;

template <class T, class = void> struct has_tag : std::false_type {};
template <class T> struct has_tag<T, void_t<typename tag_of<T>::type>> : std::true_type {};
} // namespace meta

} // namespace tmdesc