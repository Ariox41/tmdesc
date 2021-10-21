// Copyright Victor Smirnov 2021
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)

#pragma once

#include "../tuple/tuple.hpp"
#include "../flag_map.hpp"
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
    template <class... Flags> constexpr flag_map<Flags...> flags(const Flags&... flags) const noexcept {
        return {flags...};
    }

    template <class M, class U, class... Flags>
    constexpr simple_member_info_pack_for_memptr_t<M, T> member(zstring_view name, M U::*member) const noexcept {
        static_assert(std::is_base_of<U, T>{}, "the member must be a pointer to member of T or its base class");
        M T::*real_memptr = member;
        return {name, real_memptr};
    }

    template <class M, class U, class... Flags>
    constexpr full_member_info_pack_for_memptr_t<M, T, Flags...> member(zstring_view name, M U::*member,
                                                                const flag_map<Flags...>& member_flags) const noexcept {
        static_assert(std::is_base_of<U, T>{}, "the member must be a pointer to member of T or its base class");
        M T::*real_memptr = member;
        return {name, real_memptr, member_flags};
    }

    template <class... Members>
    constexpr type_info_pack<tuple<Members...>> operator()(const Members&... members) const noexcept {
        return {tuple<Members...>{members...}};
    }

    template <class... Members, class... Flags>
    constexpr type_info_pack_with_flags<tuple<Members...>, flag_map<Flags...>>
    operator()(const member_info_pack<Members>&... members, const flag_map<Flags...>& type_flags) const noexcept {
        return {tuple<Members...>{members...}, type_flags};
    }
};

template <class T> constexpr auto type_info_v = tmdesc_info(type_t<T>{}, info_builder<default_info_builder<T>>{});

} // namespace detail

template <class T> constexpr auto type_members_info_v = detail::type_info_v<T>.members();
template <class T> using type_members_info_t = meta::remove_cvref_t<decltype (type_members_info_v<T>)>;

template <class T> constexpr auto type_flags_v        = detail::type_info_v<T>.flags();

template <class T, class = void> struct has_type_info : std::false_type {};
template <class T>
struct has_type_info<T, meta::void_t<decltype(tmdesc_info(type_t<T>{}, info_builder<detail::default_info_builder<T>>{}))>>
  : std::true_type {};

template <class T> constexpr bool has_type_info_v = has_type_info<T>::value;

template <class T> struct type_members_count : public tuple_size<decltype(type_members_info_v<T>)> {};
template <class T> constexpr std::size_t type_members_count_v = type_members_count<T>::value;

} // namespace tmdesc
