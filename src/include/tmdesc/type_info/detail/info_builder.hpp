// Copyright Victor Smirnov 2021-2022
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
//
// The documentation can be found at the library's page:
// https://github.com/Ariox41/tmdesc

#pragma once
#include "../../string_view.hpp"
#include "../../tmdesc_fwd.hpp"
#include "../member_info.hpp"
#include "../type_info.hpp"
#include <boost/hana/map.hpp>
#include <boost/hana/optional.hpp>
#include <boost/hana/tuple.hpp>
namespace tmdesc {
struct _default {};
namespace hana = boost::hana;

namespace detail {
template <class M, class O> struct memptr_function_object {
    explicit constexpr memptr_function_object(M O::*member_ptr) noexcept
      : member_ptr_(member_ptr) {}

    using member_type = M;

    constexpr const M& operator()(const O& owner) const noexcept { return owner.*member_ptr_; }
    constexpr M& operator()(O& owner) const noexcept { return owner.*member_ptr_; }
    constexpr M&& operator()(O&& owner) const noexcept { return std::move(owner).*member_ptr_; }
    constexpr const M&& operator()(const O&& owner) const noexcept { return std::move(owner).*member_ptr_; }

private:
    M O::*member_ptr_ = nullptr;
};
} // namespace detail

template <class T> class info_builder<T, _default> {
public:
    template <class U> struct attribute_set {
        using type = U;
        U attributes;
    };
    template <class U> struct member_set_info {
        using type = U;
        U members;
    };

    // wrap typename string to attribute
    constexpr attribute<tags::type_name, zstring_view> type_name(zstring_view name) const noexcept { return {name}; }

    // wraps attributes to attribute_set
    template <class... KS, class... VS>
    constexpr attribute_set<hana::map<hana::pair<hana::type<KS>, VS>...>>
    attributes(attribute<KS, VS>... attributes) const {
        return {hana::make_map(hana::make_pair(hana::type_c<KS>, attributes.value)...)};
    }

    // wraps information about a member
    // @param name - member name
    // @param member - pointer to member of target type or pointer to member of target base type
    template <class M, class U> constexpr auto member(zstring_view name, M U::*member) const {
        static_assert(std::is_base_of<U, T>{}, "the member must be a pointer to member of T or its base class");
        M T::*real_memptr = member;
        return detail::member_info<M, T, hana::map<>>{name, real_memptr, hana::map<>{}};
    }

    // wraps information about a member
    // @param name - member name
    // @param member - pointer to member of target type or pointer to member of target base type
    // @param attributes_ - member attributes, the result of the `attributes` function.
    template <class M, class U, class AS>
    constexpr auto member(zstring_view name, M U::*member, attribute_set<AS> attributes_) const {
        static_assert(std::is_base_of<U, T>{}, "the member must be a pointer to member of T or its base class");
        M T::*real_memptr = member;
        return member_info<M, T, AS>{name, real_memptr, std::move(attributes_.value)};
    }

    // wraps information about member set to single struct
    template <class... M, class... T, class... A>
    constexpr member_set_info<hana::tuple<member_info<M, T, A>...>> members(member_info<M, T, A>... members_) const {
        return {haba::make_tuple(std::move(members))};
    }

    // wraps information about type set to single struct
    // @param member_set_ - type members info,  the result of the `members` function
    template <class M> constexpr type_info<hana::optional<M>, hana::map<>> type(member_set_info<M> member_set_) const {
        return {hana::just(std::move(member_set_)), hana::map<>{}};
    }

    // wraps information about type set to single struct
    // @param attributes_ - type attributes, the result of the `attributes` function.
    template <class AS> constexpr type_info<hana::optional<>, AS> type(attribute_set<AS> attributes_) const {
        return {hana::nothing, std::move(attributes_.value)};
    }

    // wraps information about type set to single struct
    // @param member_set_ - type members info,  the result of the `members` function
    // @param attributes_ - type attributes, the result of the `attributes` function.
    template <class AS, class M>
    constexpr type_info<hana::optional<M>, AS> type(attribute_set<AS> attributes_,
                                                    member_set_info<M> member_set_) const noexcept {
        return {hana::just(std::move(member_set_)), std::move(attributes_)};
    }
};

} // namespace tmdesc
