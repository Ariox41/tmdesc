// Copyright Victor Smirnov 2021-2022
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
//
// The documentation can be found at the library's page:
// https://github.com/Ariox41/tmdesc

#pragma once
#include "meta/void_t.hpp"
#include "type_info/detail/info_builder.hpp"
#include <boost/hana/chain.hpp>
#include <boost/hana/transform.hpp>
namespace tmdesc {

template <typename T, typename = void> struct get_type_info_impl {
    static constexpr boost::hana::optional<> apply() { return boost::hana::nothing; }
};

namespace detail {
template <class T> constexpr  auto type_info_cache = ::tmdesc::get_type_info_impl<T>::apply();

struct get_members_fn {
    template <typename T> constexpr const auto& operator()(const T& ti) const {
        return ti.members();
    }
};

struct get_attributes_fn {
    template <typename T> constexpr const auto& operator()(const T& ti) const{
        return ti.attributes();
    }
};
} // namespace detail

struct get_type_info_t {
    template <class T> constexpr const auto& operator()(boost::hana::basic_type<T>) const {
        return ::tmdesc::detail::type_info_cache<T>;
    }
};
struct get_type_members_info_t {
    template <class T> constexpr const auto& operator()(boost::hana::basic_type<T>) const {
        return boost::hana::maybe(boost::hana::nothing, ::tmdesc::detail::get_members_fn{},
                                  ::tmdesc::detail::type_info_cache<T>);
    }
};
struct get_type_attributes_t {
    template <class T> constexpr const auto& operator()(boost::hana::basic_type<T>) const {
        return boost::hana::maybe(boost::hana::nothing, ::tmdesc::detail::get_attributes_fn{},
                                  ::tmdesc::detail::type_info_cache<T>);
    }
};

/**
 * @return an optional value of type @ref type_info_wrapper
 *
 * @details
 * If the `tmdesc_info(info_builder<T, unspecified>)` free function is implemented for type T,
 * then return `just(type_info_wrapper<unspecified>{})`.
 * Otherwise, return `nothing`.
 */
constexpr get_type_info_t get_type_info{};

/**
 * @return an optional tuple of @ref member_info
 *
 * @details
 * If the `tmdesc_info(info_builder<T, unspecified>)` free function is implemented for type T,
 * and type description contains members set,
 * then the return `just(tuple<member_info<unspecified>...>{})`.
 * Otherwise, return `nothing`.
 */
constexpr get_type_members_info_t get_type_members_info{};

/**
 * @return an optional value `map` of type attributes.
 *
 * @details
 * If the `tmdesc_info(info_builder<T, unspecified>)` free function is implemented for type T (see `tmdesc_fwd.hpp`),
 * then return `just(map<pair<Tag, Value>...>)`.
 * Otherwise, return `nothing`.
 */
constexpr get_type_attributes_t get_type_attributes{};

template <class T>
struct get_type_info_impl<T, meta::void_t<decltype(tmdesc_info(::tmdesc::info_builder<T, ::tmdesc::_default>{}))>> {
    // Call free function `tmdesc_info(info_builder<unspecified>{})`, and return the result.
    // see [Argument-dependent lookup](https://en.cppreference.com/w/cpp/language/adl)
    // The arguments of the info_builder template contain the desired type, which means that
    // if a free function is defined in the type namespace, it will be called correctly.
    static constexpr auto apply() {
        return boost::hana::just(tmdesc_info(::tmdesc::info_builder<T, ::tmdesc::_default>{}));
    }
};

} // namespace tmdesc
