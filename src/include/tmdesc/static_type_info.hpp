// Copyright Victor Smirnov 2021
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
//
// The documentation can be found at the library's page:
// https://github.com/Ariox41/tmdesc

#pragma once
#include "detail/default_info_builder.hpp"
namespace tmdesc {

template <class T>
constexpr auto static_type_info_v = tmdesc_info(type_t<T>{},
                                                info_builder<detail::default_info_builder<T>>{});

/// The tuple of ::tmdesc::member_info_pack objects fot type T
template <class T> constexpr auto static_members_info_v = static_type_info_v<T>.members();

template <std::size_t I, class T>
using member_type_at_t =
    typename meta::remove_cvref_t<decltype(get<I>(static_members_info_v<T>))>::member_type;

/// The ::tmdesc::flag_map for type T (empty flag_map by default if has_type_info)
template <class T> constexpr auto static_type_flags_v = static_type_info_v<T>.flags();

template <class T, class = void> struct has_type_info : std::false_type {};
template <class T>
struct has_type_info<                                                                   //
    T, meta::void_t<decltype(                                                           //
           tmdesc_info(type_t<T>{}, info_builder<detail::default_info_builder<T>>{}))>> //
  : std::true_type {};                                                                  //

template <class T> constexpr bool has_type_info_v = has_type_info<T>::value;

template <class T>
struct type_members_count : public tuple_size<decltype(static_members_info_v<T>)> {};

template <class T> constexpr std::size_t type_members_count_v = type_members_count<T>::value;

} // namespace tmdesc
