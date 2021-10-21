// Copyright Victor Smirnov 2021
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)

#pragma once
#include "algorithm/for_each.hpp"
#include "static_type_info.hpp"

namespace tmdesc {
template <class Owner, std::size_t I> struct member_reference {
    using owner_type     = meta::remove_cvref_t<Owner>;
    using reference_type = decltype(get<I>(static_members_info_v<owner_type>).get_ref(std::declval<Owner>()));
    using value_type     = member_type_at_t<I, owner_type>;

private:
    Owner owner_;

public:
    explicit constexpr member_reference(Owner owner) noexcept
      : owner_(owner){};
    constexpr member_reference(const member_reference&) = default;
    constexpr member_reference& operator=(const member_reference&) = delete;

    constexpr reference_type get() const noexcept {
        return ::tmdesc::get<I>(static_members_info_v<owner_type>).get_ref(static_cast<Owner&&>(owner_));
    }
    static constexpr string_view name() noexcept { return ::tmdesc::get<I>(static_members_info_v<owner_type>).name(); }
    static constexpr std::size_t index() noexcept { return I; }
    static constexpr decltype(auto) flags() noexcept {
        return ::tmdesc::get<I>(static_members_info_v<owner_type>).flags();
    }
};

struct visit_members_t {
private:
    struct impl_t {
        template <class Struct, class Fn, std::size_t... I>
        constexpr void operator()(Struct&& s, Fn&& fn, std::index_sequence<I...>) const
            noexcept(meta::fast_and({noexcept(invoke(std::declval<Fn&>(),
                                                     member_reference<Struct&&, I>{std::declval<Struct>()}))...})) {

            (void)std::initializer_list<bool>{
                true, ((void)invoke(static_cast<Fn&>(fn), member_reference<Struct&&, I>{std::forward<Struct>(s)}),
                       void(), true)...};
        }
    };

public:
    template <class S, class Visitor, std::enable_if_t<has_type_info_v<meta::remove_cvref_t<S>>, bool> = true>
    constexpr void operator()(S&& s, Visitor&& vis) const
        noexcept(noexcept(impl_t{}(std::declval<S>(), std::declval<Visitor>(),
                                   std::make_index_sequence<type_members_count_v<meta::remove_cvref_t<S>>>{}))) {
        return impl_t{}(std::forward<S>(s), std::forward<Visitor>(vis),
                        std::make_index_sequence<type_members_count_v<meta::remove_cvref_t<S>>>{});
    }
};
constexpr visit_members_t visit_members{};

} // namespace tmdesc
