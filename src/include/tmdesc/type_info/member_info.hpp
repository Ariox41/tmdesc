// Copyright Victor Smirnov 2021-2023
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
//
// The documentation can be found at the library's page:
// https://github.com/Ariox41/tmdesc
#pragma once
#include <boost/hana/map.hpp>
#include <tmdesc/hana_fwd.hpp>
#include <tmdesc/string_view.hpp>
namespace tmdesc {

/**
 * @brief  Contains information about some type member.
 * @tparam Accessor - member accessor type.
 * @tparam Attributes - member attributes type, hana::map{type<Attribute> -> AttributeValue}.
 * @note The type of the member is generally unknown, since the member can be a reference.
 * The type can be obtained as a result of calling accessor.
 */
template <typename Accessor, typename Attributes> struct member_info {
    constexpr member_info(zstring_view name, Accessor accessor, Attributes attributes)
      : member_name_(name)
      , accessor_(std::move(accessor))
      , attributes_(std::move(attributes)) {}

    /// @return member name.
    constexpr zstring_view name() const noexcept { return member_name_; }

    /// @return invocable object for getting a reference to the member from the owner object.
    constexpr const Accessor& accessor() const noexcept { return accessor_; }

    /// @return `map<pair<type<Attribute>, Value>...>` of member attributes
    constexpr const Attributes& attributes() const noexcept { return attributes_; }

private:
    zstring_view member_name_;
    Accessor accessor_;
    Attributes attributes_;
};
} // namespace tmdesc
