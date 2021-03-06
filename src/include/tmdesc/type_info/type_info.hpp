// Copyright Victor Smirnov 2021-2022
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
//
// The documentation can be found at the library's page:
// https://github.com/Ariox41/tmdesc
#include "../string_view.hpp"
#include <boost/hana/optional.hpp>
namespace tmdesc {

/** Contains information about type T.
    A type can provide information about the set of its members and information about specific attributes of the type.

    Type attributes are represented as a map {tag->value}, where tag is the type (identifier)
    of the attribute, value is an arbitrary value specified for this attribute.
    The set of attributes may be empty.

    The set of members is represented as `nothing` or `just(tuple<member_info<unspecified>>{})`.
    There is a difference between an empty list of members, and no list.
    For example, a structure with an empty list of members corresponds to the xml representation `<Struct Name />`.
    If the list of members is not specified, this structure cannot be serialized in xml.
    But another sterilization method can be specified in the type attributes.
*/
template <class T, class MS, class AS> struct type_info {
    constexpr type_info(MS members, AS attributes)
      : members_(std::move(members))
      , attributes_(std::move(attributes)) {}

    /// @return `just(tuple(members...)))` or `nothing`.
    /// @details the members is an optional tuple of @ref member_info objects.
    constexpr const MS& members() const noexcept { return members_; }

    /// @return `map<pair<Tag, Value>...>` of type attributes.
    /// @note the optional default attribute has tag of @ref tags::type_name with value of type `zstring_view`.
    constexpr const AS& attributes() const noexcept { return attributes_; }
private:
    MS members_;
    AS attributes_;
};

} // namespace tmdesc
