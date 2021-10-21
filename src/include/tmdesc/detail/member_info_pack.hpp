// Copyright Victor Smirnov 2021
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)

#pragma once

#include "../meta/helpers.hpp"
#include "../type_description.hpp"
#include "../flag_map.hpp"
namespace tmdesc {


namespace detail {

template <class MemberType, class OwnerType> struct simple_memptr_info {
private:
    using owner_type      = OwnerType;
    using member_ptr_type = MemberType OwnerType::*;

    const zstring_view member_name;
    const member_ptr_type member_ptr;

public:
    using member_type = MemberType;

    constexpr simple_memptr_info(zstring_view name, MemberType OwnerType::* memptr)
      : member_name(name)
      , member_ptr(memptr) {}

    constexpr zstring_view name() const noexcept { return member_name; }
    constexpr const member_type& get_ref(const owner_type& owner) const noexcept { return owner.*member_ptr; }
    constexpr member_type& get_ref(owner_type& owner) const noexcept { return owner.*member_ptr; }
    constexpr member_type&& get(owner_type&& owner) const noexcept { return owner.*member_ptr; }

    constexpr flag_map<> flags() const noexcept { return {}; }

    template <class Rha> constexpr bool operator==(const Rha& rha) const noexcept { return eqImpl(&rha); }
    template <class Rha> constexpr bool operator!=(const Rha& rha) const noexcept { return !eqImpl(&rha); }

private:
    constexpr bool eqImpl(const simple_memptr_info* rha) const noexcept { return member_ptr == rha->member_ptr; }
    constexpr bool eqImpl(const void*) const noexcept { return false; }
};

template <class MemberType, class OwnerType, class... Flags>
struct full_member_info_pack : public simple_memptr_info<MemberType, OwnerType> {
    using super_t         = simple_memptr_info<MemberType, OwnerType>;

    const flag_map<Flags...> flags_;

public:
    constexpr full_member_info_pack(zstring_view name, MemberType OwnerType::* memptr, flag_map<Flags...> flags)
      : super_t(name, memptr)
      , flags_(flags) {}

    constexpr const flag_map<Flags...>& flags() const noexcept { return flags_; }
};

template <class MemberType, class OwnerType>
using simple_member_info_pack_for_memptr_t = ::tmdesc::member_info_pack<simple_memptr_info<MemberType, OwnerType>>;

template <class MemberType, class OwnerType, class... Flags>
using full_member_info_pack_for_memptr_t = ::tmdesc::member_info_pack<full_member_info_pack<MemberType, OwnerType, Flags...>>;
} // namespace detail

} // namespace tmdesc
