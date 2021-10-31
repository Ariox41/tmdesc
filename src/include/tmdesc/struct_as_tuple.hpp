// Copyright Victor Smirnov 2021
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
//
// The documentation can be found at the library's page:
// https://github.com/Ariox41/tmdesc

#pragma once
#include "static_type_info.hpp"
namespace tmdesc {

/// The tuple_size of a type that `has_type_info` is equal to the number of type members
template <class T>
struct tuple_size<T, std::enable_if_t<has_type_info_v<T>>>
  : public tuple_size<decltype(static_members_info_v<std::decay_t<T>>)> {};

namespace detail {
struct getter_for_struct_t {
    template <std::size_t I, class T> static constexpr decltype(auto) get_by_id(T&& s) noexcept {
        constexpr auto member_getter = get<I>(static_members_info_v<std::decay_t<T>>);
        return member_getter.get(std::forward<T>(s));
    }

    template <std::size_t I, class T>
    using tuple_element =
        typename tuple_element_t<I, decltype(static_members_info_v<T>)>::member_type;
};
} // namespace detail

/// The tuple_getter for a type that `has_type_info` allows you to get
/// a reference to a member by index
/// @note The index is determined by the order in the tmdesc_info for type, which may not correspond
/// to the actual order of placement in memory.
/// It can be a problem if order is important, for example, for comparison operators implementation.
template <class T> struct tuple_getter<T, std::enable_if_t<has_type_info_v<T>>> {
    using type = detail::getter_for_struct_t;
};

} // namespace tmdesc
