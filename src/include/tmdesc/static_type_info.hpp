// Copyright Victor Smirnov 2021
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
//
// The documentation can be found at the library's page:
// https://github.com/Ariox41/tmdesc

#pragma once
#include "detail/default_info_builder.hpp"
namespace tmdesc {

// Prefer to use `static_members_info_v` or `static_type_flags_v`
template <class T>
constexpr auto static_type_info_v = tmdesc_info(type_t<T>{},
                                                info_builder<detail::default_info_builder<T>>{});

/// A tuple of objects of type @ref member_info_pack
/// that contain information about members of type `T`
template <class T> constexpr auto static_members_info_v = static_type_info_v<T>.members();



/// The @ref flag_map for type T (empty flag_map by default if has_type_info<T>)
template <class T> constexpr auto static_type_flags_v = static_type_info_v<T>.flags();

/// The `has_type_info` is `true_type` if call of
/// `tmdesc_info(type_t<T>{}, info_builder</*implementation defined*/>{})` is well formed
template <class T, class = void> struct has_type_info : std::false_type {};
template <class T>
struct has_type_info<                                                                   //
    T, meta::void_t<decltype(                                                           //
           tmdesc_info(type_t<T>{}, info_builder<detail::default_info_builder<T>>{}))>> //
  : std::true_type {};                                                                  //

/// An alias for has_type_info<T>::value
template <class T> constexpr bool has_type_info_v = has_type_info<T>::value;

// Prefer to use tuple_element_t in `struct_as_tuple.hpp`
template <std::size_t I, class T>
using member_type_at_t =
    typename meta::remove_cvref_t<decltype(get<I>(static_members_info_v<T>))>::member_type;

// Prefer to use tuple_size in `struct_as_tuple.hpp`
template <class T>
struct type_members_count : public tuple_size<decltype(static_members_info_v<T>)> {};

// Prefer to use tuple_size_v in `struct_as_tuple.hpp`
template <class T> constexpr std::size_t type_members_count_v = type_members_count<T>::value;

} // namespace tmdesc
