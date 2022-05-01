// Copyright Victor Smirnov 2021
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
//
// The documentation can be found at the library's page:
// https://github.com/Ariox41/tmdesc

#pragma once
#include "detail/info_builder.hpp"
#include <boost/hana/chain.hpp>
#include <boost/hana/transform.hpp>
namespace tmdesc {
namespace detail {
struct get_type_info_impl {
    template <class T>
    constexpr auto operator()(hana::type<T>) const -> decltype(tmdesc_info(info_builder<T, _default>{})) {
        return tmdesc_info(info_builder<T, _default>{});
    }
};
constexpr get_type_info_impl get_type_info{};

struct get_members_info_t {
    template <class T> constexpr auto operator()(const T& type_info) const -> decltype(std::declval<T>().members()) {
        return type_info.members();
    }
};
constexpr get_members_info_t get_members_info{};

struct get_attributes_t {
    template <class T> constexpr auto operator()(const T& type_info) const -> decltype(std::declval<T>().attributes()) {
        return type_info.attributes();
    }
};
constexpr get_attributes_t get_attributes{};

} // namespace detail

/** Contains an optional value of type @ref type_info

    If the `tmdesc_info(info_builder<T, unspecified>)` free function is implemented for type T (see `tmdesc_fwd.hpp`),
    then the value is `just(type_info<unspecified>{})`.
    Otherwise, the value is `nothing`.
*/
template <class T>
constexpr auto static_type_info_v = boost::hana::sfinae(detail::get_type_info)(boost::hana::type_c<T>);

/** Contains an optional tuple of @ref member_info

    If the `tmdesc_info(info_builder<T, unspecified>)` free function is implemented for type T (see `tmdesc_fwd.hpp`),
    and type description contains members set, then the value is `just(tupe<member_info<unspecified>...>{})`.
    Otherwise, the value is `nothing`.
*/
template <class T>
constexpr auto static_type_members_v = boost::hana::chain(static_type_info_v<T>, detail::get_members_info);

/** Contains an optional map of attributes.

    If the `tmdesc_info(info_builder<T, unspecified>)` free function is implemented for type T (see `tmdesc_fwd.hpp`),
    then the value is `just(map<pair<type<Tags>, Value>...>{})`.
    Otherwise, the value is `nothing`.
 */
template <class T>
constexpr auto static_type_attributes_v = boost::hana::transform(static_type_info_v<T>, detail::get_attributes);

} // namespace tmdesc
