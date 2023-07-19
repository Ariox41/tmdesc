// Copyright Victor Smirnov 2021-2022
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
//
// The documentation can be found at the library's page:
// https://github.com/Ariox41/tmdesc

#pragma once
#include <boost/hana/basic_tuple.hpp>
#include <boost/hana/map.hpp>
#include <boost/hana/pair.hpp>
#include <tmdesc/functional/invoke.hpp>
#include <tmdesc/hana_fwd.hpp>
#include <tmdesc/string_view.hpp>
#include <tmdesc/tmdesc_fwd.hpp>
#include <tmdesc/type_info/member_info.hpp>
#include <tmdesc/type_info/type_info.hpp>
namespace tmdesc {
struct _default {};
namespace detail {
template <class U> struct attribute_set {
    using type = U;
    U attributes;
};
template <class As> constexpr auto make_attribute_set(As as) { return attribute_set<As>{as}; }

template <class U> struct member_set_info {
    using type = U;
    U members;
};
template <class Ms> constexpr auto make_members_set(Ms as) { return member_set_info<Ms>{as}; }
} // namespace detail

template <class T> class info_builder<T, _default> {
public:
    /// wraps type attributes
    template <typename... Keys, typename... Values>
    constexpr auto attributes(tmdesc::attribute<Keys, Values>... attributes) const {
        return tmdesc::detail::make_attribute_set(
            hana::make_map(hana::make_pair(hana::type_c<Keys>, attributes.value)...));
    }

    /// wraps information about members
    template <typename... ACS, typename... ATS>
    constexpr auto members(tmdesc::member_info<ACS, ATS>... members_) const {
        return tmdesc::detail::make_members_set(hana::make_basic_tuple(members_...));
    }

    /// wraps information about a member
    /// @param name - member name
    /// @param member - pointer to member of target type or pointer to member of target base type or other similar invocable object
    /// @param attributes - member attributes.
    template <typename AC, typename... Keys, typename... Values, std::enable_if_t<is_invocable_v<AC, T>, bool> = true>
    constexpr auto member(zstring_view name, AC member_accessor, attribute<Keys, Values>... attributes) const {
        using AttributeMap = hana::map<hana::pair<hana::type<Keys>, Values>...>;
        return member_info<AC, AttributeMap>{name, member_accessor,
                                             hana::make_map(hana::make_pair(hana::type_c<Keys>, attributes.value)...)};
    }

    /// wraps information about a member
    /// @param name - member name
    /// @param member - pointer to member of target type or pointer to member of target base type or other similar invocable object
    /// @param attributes - member attributes as result of info_builder::attributes() call.
    template <typename AC, typename AS, std::enable_if_t<is_invocable_v<AC, T>, bool> = true>
    constexpr auto member(zstring_view name, AC member_accessor, detail::member_set_info<AS> attributes) const {
        return member_info<AC, AS>{name, member_accessor, attributes.attributes};
    }

    /// wraps information about type
    template <typename AS, typename MS>
    constexpr auto build(detail::attribute_set<AS> attributes, detail::member_set_info<MS> members) const {
        return tmdesc::type_info<T, MS, AS>{members.members, attributes.attributes};
    }
    /// wraps information about type
    template <typename AS> constexpr auto build(detail::attribute_set<AS> attributes) const {
        return tmdesc::type_info<T, hana::basic_tuple<>, AS>(hana::basic_tuple<>{}, attributes.attributes);
    }
    /// wraps information about type
    template <typename MS> constexpr auto build(detail::member_set_info<MS> members) const {
        return tmdesc::type_info<T, MS, hana::map<>>{members.members, hana::map<>{}};
    }
};

} // namespace tmdesc
