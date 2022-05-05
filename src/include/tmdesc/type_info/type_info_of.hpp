// Copyright Victor Smirnov 2021-2022
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
struct type_info_of_impl {
    // Call free function `tmdesc_info(info_builder<unspecified>{})`, and return the result.
    // see [Argument-dependent lookup](https://en.cppreference.com/w/cpp/language/adl)
    // The arguments of the info_builder template contain the desired type, which means that
    // if a free function is defined in the type namespace, it will be called correctly.
    template <class T>
    constexpr auto operator()(boost::hana::basic_type<T>) const -> decltype(tmdesc_info(info_builder<T, _default>{})) {
        return tmdesc_info(info_builder<T, _default>{});
    }
};
constexpr type_info_of_impl type_info_of{};
struct get_members_t {
    template <class T> constexpr decltype(auto) operator()(const T& ti) const { return ti.members(); }
};
constexpr get_members_t get_members{};

struct get_attributes_t {
    template <class T> constexpr decltype(auto) operator()(const T& ti) const { return ti.attributes(); }
};
constexpr get_attributes_t get_attributes{};

// Cache of type information.
// The value must be calculated at most once for a translation unit.
template <class T> constexpr auto type_info_cache = boost::hana::sfinae(type_info_of)(boost::hana::type_c<T>);

// Cache of type members.
// The value must be calculated at most once for a translation unit.
template <class T> constexpr auto type_members_cache = boost::hana::chain(type_info_cache<T>, get_members);

// Cache of type attributes.
// The value must be calculated at most once for a translation unit.
template <class T> constexpr auto type_attributes_cache = boost::hana::transform(type_info_cache<T>, get_attributes);

// Cache of type members, if type has members.
// The value must be calculated at most once for a translation unit.
template <class T> constexpr auto&& members_cache_unchecked = type_members_cache<T>.value();

// Cache of type attributes, if type has attributes.
// The value must be calculated at most once for a translation unit.
template <class T> constexpr auto&& attributes_cache_unchecked = type_attributes_cache<T>.value();

} // namespace detail
struct type_info_of_t {
    template <class T>
    constexpr auto operator()(boost::hana::basic_type<T>) const noexcept -> decltype(*&detail::type_info_cache<T>) {
        return detail::type_info_cache<T>;
    }
};
struct type_members_info_of_t {
    template <class T>
    constexpr auto operator()(boost::hana::basic_type<T>) const noexcept -> decltype(*&detail::type_members_cache<T>) {
        return detail::type_members_cache<T>;
    }
};
struct type_attributes_of_t {
    template <class T>
    constexpr auto operator()(boost::hana::basic_type<T>) const noexcept
        -> decltype(*&detail::type_attributes_cache<T>) {
        return detail::type_attributes_cache<T>;
    }
};

/**
 * @return an optional value of type @ref type_info
 *
 * @details
 * If the `tmdesc_info(info_builder<T, unspecified>)` free function is implemented for type T (see `tmdesc_fwd.hpp`),
 * then return `just(type_info<unspecified>{})`.
 * Otherwise, return `nothing`.
 */
constexpr type_info_of_t type_info_of{};

/**
 * @return an optional tuple of @ref member_info
 *
 * @details
 * If the `tmdesc_info(info_builder<T, unspecified>)` free function is implemented for type T (see `tmdesc_fwd.hpp`),
 * and type description contains members set,
 * then the return `just(tuple<member_info<unspecified>...>{})`.
 * Otherwise, return `nothing`.
 */
constexpr type_members_info_of_t type_members_info_of{};

/**
 * @return an optional value `map` of type attributes.
 *
 * @details
 * If the `tmdesc_info(info_builder<T, unspecified>)` free function is implemented for type T (see `tmdesc_fwd.hpp`),
 * then return `just(map<pair<Tag, Value>...>)`.
 * Otherwise, return `nothing`.
 */
constexpr type_attributes_of_t type_attributes_of{};

} // namespace tmdesc
