// Copyright Victor Smirnov 2021-2022
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
//
// The documentation can be found at the library's page:
// https://github.com/Ariox41/tmdesc
#include "../string_view.hpp"
#include <boost/hana/optional.hpp>
namespace tmdesc {

/**
 * @brief Contains information about type T.
 *
 * @details
 * A type can provide information about the set of its members and information about specific attributes of the type.
 * Type attributes are represented as a map `{tag->value}`, where tag is the type (identifier)
 * of the attribute, and value is an arbitrary value specified for this attribute.
 * The set of attributes may be empty.
 * 
 * @note 
 * The set of members is represented as `nothing` or `just(tuple<member_info<unspecified>>{})`.
 * There is a difference between an empty list of members, and no list.
 * For example, a structure with an empty list of members corresponds to the xml representation `<StructName />`.
 * If the list of members is not specified, this structure cannot be serialized in xml.
 * But another serialization method can be specified in the type attributes.
 */
template <class T, class MS, class AS> struct type_info_wrapper {
    constexpr type_info_wrapper(MS members, AS attributes)
      : members_(std::move(members))
      , attributes_(std::move(attributes)) {}

    /**
     * @brief the members is an optional tuple of @ref member_info objects.
     * @return `just(tuple(members...)))` or `nothing`.
     */
    constexpr const MS& members() const noexcept { return members_; }

    /// @return `map<pair<Tag, Value>...>` of type attributes.
    /// @note If a type name is specified in the description, it is available by the `tags::type_name` tag, its value is of the `zstring_view` type
    constexpr const AS& attributes() const noexcept { return attributes_; }

private:
    MS members_;
    AS attributes_;
};

} // namespace tmdesc
