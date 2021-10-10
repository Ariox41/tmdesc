// Copyright Victor Smirnov 2021
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)

#pragma once

#ifdef DOXYGEN
#define TMDESC_DOXYGEN_AUTO(...) __VA_ARGS__
#else
#define TMDESC_DOXYGEN_AUTO(...) auto
#endif

namespace tmdesc {
namespace meta {

/// type with value-semantic
template <class T> struct type_t { using type = T; };

/// type list with value-semantic
template <class... T> struct type_list {};

///// see std::void_t
//template <class...> using void_t = void;
template<typename... Ts> struct make_void { typedef void type;};
template<typename... Ts> using void_t = typename make_void<Ts...>::type;

} // namespace meta
} // namespace tmdesc
