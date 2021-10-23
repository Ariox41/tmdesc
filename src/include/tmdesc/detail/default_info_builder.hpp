// Copyright Victor Smirnov 2021
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
//
// The documentation can be found at the library's page:
// https://github.com/Ariox41/tmdesc

#pragma once

#include "../flag_map.hpp"
#include "../tuple/tuple.hpp"
#include "member_info_pack.hpp"
namespace tmdesc {

namespace detail {

template <class Members> struct type_info_pack {
private:
    Members members_;

public:
    using members_type = Members;
    using flags_type   = flag_map<>;

    constexpr type_info_pack()                      = default;
    constexpr type_info_pack(const type_info_pack&) = default;

    constexpr type_info_pack(Members&& m) noexcept
      : members_(std::move(m)) {}
    constexpr const members_type& members() const noexcept { return members_; }
    constexpr flags_type flags() const noexcept { return {}; }
};

template <class Members, class Flags> struct type_info_pack_with_flags {
private:
    Members members_;
    Flags flags_;

public:
    using members_type = Members;
    using flags_type   = Flags;
    constexpr type_info_pack_with_flags(Members m, Flags f) noexcept
      : members_(std::move(m))
      , flags_(std::move(f)) {}
    constexpr const members_type& members() const noexcept { return members_; }
    constexpr const flags_type& flags() const noexcept { return flags_; }
};

template <class T> struct default_info_builder {
    template <class... Flags>
    constexpr flag_map<Flags...> flags(const Flags&... flags) const noexcept {
        return {flags...};
    }

    template <class M, class U, class... Flags>
    constexpr simple_member_info_pack_for_memptr_t<M, T> member(zstring_view name,
                                                                M U::*member) const noexcept {
        static_assert(std::is_base_of<U, T>{},
                      "the member must be a pointer to member of T or its base class");
        M T::*real_memptr = member;
        return {name, real_memptr};
    }

    template <class M, class U, class... Flags>
    constexpr full_member_info_pack_for_memptr_t<M, T, Flags...>
    member(zstring_view name, M U::*member, const flag_map<Flags...>& member_flags) const noexcept {
        static_assert(std::is_base_of<U, T>{},
                      "the member must be a pointer to member of T or its base class");
        M T::*real_memptr = member;
        return {name, real_memptr, member_flags};
    }

    template <class... Members>
    constexpr type_info_pack<tuple<Members...>>
    operator()(const Members&... members) const noexcept {
        return {tuple<Members...>{members...}};
    }

    template <class... Members, class... Flags>
    constexpr type_info_pack_with_flags<tuple<Members...>, flag_map<Flags...>>
    operator()(const member_info_pack<Members>&... members,
               const flag_map<Flags...>& type_flags) const noexcept {
        return {tuple<Members...>{members...}, type_flags};
    }
};

} // namespace detail

} // namespace tmdesc
