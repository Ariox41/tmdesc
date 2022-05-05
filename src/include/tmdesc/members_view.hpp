// Copyright Victor Smirnov 2021-2022
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
//
// The documentation can be found at the library's page:
// https://github.com/Ariox41/tmdesc

#pragma once
#include "type_info/get_type_info.hpp"
#include <boost/hana/tuple.hpp>

namespace tmdesc {
template <class T> struct object_members_view {
    using owner_object_type = T;
    owner_object_type owner_object;

    object_members_view(const object_members_view&) = delete;
};

struct members_view_t {
    template <class T, std::enable_if_t<!boost::hana::is_nothing(static_type_members_v<std::decay_t<T>>), bool> = true>
    object_members_view<T&&> operator()(T&& object) const noexcept {
        return {std::forward<T>(object)};
    }
};
constexpr members_view_t members_view;

namespace tags {
struct members_view_tag {};
} // namespace tags

// namespace detail {
//template <class T> constexpr const auto& existing_members_info_v = ::tmdesc::static_type_members_v<T>.value();

// template <std::size_t I, class T>
//constexpr const auto& existing_info_of_member_at_v = hana::at_c<I>(existing_members_info_v<T>);

// template <std::size_t I, class T>
//constexpr const auto& existing_member_getter_at_v = existing_info_of_member_at_v<I, T>.getter();

// template <std::size_t I, class T>
//using existing_member_type_at = typename std::decay_t<decltype(existing_info_of_member_at_v<I, T>)>::value_type;
//} // namespace detail

///// The reference to object member with name and attributes.
// template <class Owner, std::size_t I> struct member_reference {
//     using owner_type     = std::decay_t<Owner>;
//    using reference_type = decltype(detail::existing_member_getter_at_v<I, owner_type>(std::declval<Owner>()));
//    using value_type     = detail::existing_member_type_at<I, owner_type>;

// private:
//     Owner owner_;

// public:
//    explicit constexpr member_reference(Owner owner) noexcept
//      : owner_(owner){};
//    constexpr member_reference(const member_reference&) = default;
//    constexpr member_reference& operator=(const member_reference&) = delete;

//    /// \return reference to member. The member type qualifiers depend on the object type qualifiers
//    constexpr reference_type get() const noexcept {
//        return detail::existing_member_getter_at_v<I, owner_type>(static_cast<Owner&&>(owner_));
//    }

//    /// \return name of member
//    static constexpr zstring_view name() noexcept { return detail::existing_info_of_member_at_v<I, owner_type>.name(); }

//    /// \return index of member
//    static constexpr std::size_t index() noexcept { return I; }

//    /// \return attributes of member. The attributes has type of `map<pair<type<Tags>, Values>...>`
//    static constexpr decltype(auto) attributes() noexcept {
//        return detail::existing_info_of_member_at_v<I, owner_type>.attributes();
//    }
//};

namespace detail {
template <class O, class Getter, class AS> struct member_reference {
    using owner_type     = std::decay_t<O>;
    using reference_type = decltype(std::declval<Getter>()(std::declval<O>()));
    using value_type     = typename std::decay_t<Getter>::member_type;

    /// \return reference to member. The member type qualifiers depend on the object type qualifiers
    constexpr reference_type get() const noexcept {
        return ;
    }

    /// \return name of member
    static constexpr zstring_view name() noexcept { return detail::existing_info_of_member_at_v<I, owner_type>.name(); }

    /// \return index of member
    static constexpr std::size_t index() noexcept { return I; }

    /// \return attributes of member. The attributes has type of `map<pair<type<Tags>, Values>...>`
    static constexpr decltype(auto) attributes() noexcept {
        return detail::existing_info_of_member_at_v<I, owner_type>.attributes();
    }
};

} // namespace detail

} // namespace tmdesc

namespace boost {
namespace hana {
template <class T> struct tag_of<::tmdesc::members_view_t<T>> { using type = ::tmdesc::tags::members_view_tag; };
template <> struct unpack_impl<::tmdesc::tags::members_view_tag> {
    template <typename Xs, typename F> static constexpr auto apply(Xs&& xs, F&& f) {
        return unpack_impl<basic_tuple_tag>(std::forward<Xs>(xs).tup, std::forward<F>(f));
    }
};
} // namespace hana
} // namespace boost
