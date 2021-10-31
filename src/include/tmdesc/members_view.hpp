// Copyright Victor Smirnov 2021
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
//
// The documentation can be found at the library's page:
// https://github.com/Ariox41/tmdesc

#pragma once
#include "static_type_info.hpp"

namespace tmdesc {

/// The reference to object member with name and flags
/// @warning Never use this type directly, use it as an unknown type with a known interface.
/// Only the interface is stable, but not the type name and template arguments.
template <class Owner, std::size_t I> struct member_reference {
    using owner_type = meta::remove_cvref_t<Owner>;
    using reference_type =
        decltype(get<I>(static_members_info_v<owner_type>).get(std::declval<Owner>()));
    using value_type = member_type_at_t<I, owner_type>;

private:
    Owner owner_;

public:
    explicit constexpr member_reference(Owner owner) noexcept
      : owner_(owner){};
    constexpr member_reference(const member_reference&) = default;
    constexpr member_reference& operator=(const member_reference&) = delete;

    /// \return reference to member. The member type qualifiers depend on the object type qualifiers
    constexpr reference_type get() const noexcept {
        return ::tmdesc::get<I>(static_members_info_v<owner_type>)
            .get(static_cast<Owner&&>(owner_));
    }

    /// \return name of member
    static constexpr zstring_view name() noexcept {
        return ::tmdesc::get<I>(static_members_info_v<owner_type>).name();
    }

    /// \return index of member
    static constexpr std::size_t index() noexcept { return I; }

    /// \return flags of member. The flags has template type of @ref flag_map
    static constexpr decltype(auto) flags() noexcept {
        return ::tmdesc::get<I>(static_members_info_v<owner_type>).flags();
    }
};

template <class T> struct object_members_view {
    using owner_object_type = T;
    owner_object_type owner_object;

    object_members_view(const object_members_view&) = delete;
};

struct members_view_t {

    template <class T, std::enable_if_t<has_type_info_v<meta::remove_cvref_t<T>>, bool> = true>
    object_members_view<T&&> operator()(T&& object) const noexcept {
        return {std::forward<T>(object)};
    }
};
members_view_t members_view;

template <class T>
struct tuple_size<object_members_view<T>, std::enable_if_t<true>>
  : public type_members_count<meta::remove_cvref_t<T>> {};

template <class T> struct tuple_getter<object_members_view<T>, std::enable_if_t<true>> {
    struct type {
        template <std::size_t I>
        static constexpr member_reference<T, I>
        get_by_id(const object_members_view<T>& members) noexcept {
            return member_reference<T, I>{members.owner_object};
        }

        template <std::size_t I, class U>
        using tuple_element = member_reference<typename U::owner_object_type, I>;
    };
};

} // namespace tmdesc
