// Copyright Victor Smirnov 2021-2022
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
//
// The documentation can be found at the library's page:
// https://github.com/Ariox41/tmdesc
#pragma once
#include "../string_view.hpp"
#include <boost/hana/map.hpp>
namespace tmdesc {

/**
 * @brief  Contains information about data member of the type.
 */
template <class T, class Getter, class AS> struct member_info {
    using value_type = T;

    constexpr member_info(zstring_view name, Getter getter, AS attributes)
      : member_name_(name)
      , getter_(std::move(getter))
      , attributes_(std::move(attributes)) {}

    /// @return member type id
    constexpr auto type_id() const noexcept { return boost::hana::type_c<T>; }

    /// @return member name.
    constexpr zstring_view name() const noexcept { return member_name_; }

    /// @return functional object for getting a reference to the member from the owner object.
    constexpr const Getter& getter() const noexcept { return getter_; }

    /// @return `map<pair<type<Tag>, Value>...>` of member attributes
    constexpr const AS& attributes() const noexcept { return attributes_; }

private:
    zstring_view member_name_;
    Getter getter_;
    AS attributes_;
};
} // namespace tmdesc
