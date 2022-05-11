// Copyright Victor Smirnov 2021-2022
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
//
// The documentation can be found at the library's page:
// https://github.com/Ariox41/tmdesc

#pragma once
#include "type_info/type_info_of.hpp"
#include <boost/hana/range.hpp>
#include <boost/hana/transform.hpp>
namespace tmdesc {

/// Reference to a member of the struct
template <class S, std::size_t I> struct member_ref {
    explicit member_ref(S&& struct_ref)
      : struct_ref_(static_cast<S&&>(struct_ref)) {}
    static constexpr zstring_view name() noexcept {
        return boost::hana::at_c<I>(detail::members_cache_unchecked < std::decay_t<S>).name();
    }
    static constexpr auto type() noexcept {
        return boost::hana::at_c<I>(detail::members_cache_unchecked < std::decay_t<S>).type();
    }
    static constexpr decltype(auto) attributes() noexcept {
        return boost::hana::at_c<I>(detail::members_cache_unchecked < std::decay_t<S>).attributes();
    }
    constexpr decltype(auto) get() const noexcept {
        return detail::members_cache_unchecked < std::decay_t<S>).getter()(struct_ref_);
    }

private:
    S&& struct_ref_;
};

namespace detail {
template <class S> struct make_member_ref_t {
    S&& s_;
    template <class... I> constexpr auto operator()(I...) noexcept {
        // TODO tuple, or some foldable object?
        return boost::hana::make_basic_tuple(member_ref<S&&, I{}>(static_cast<S&&>(s_)));
    }
};
} // namespace detail

template <
    class S,
    std::enable_if_t<!boost::hana::is_nothing(type_members_info_of(boost::hana::type_c<std::decay_t<S>>)), bool> = true>
constexpr auto members_view(S&& s) {
    return boost::hana::unpack(
        boost::hana::make_range(boost::hana::size_c<0>,
                                boost::hana::size(detail::members_cache_unchecked<std::decay_t<S>>)),
        detail::make_member_ref_t<S&&>{static_cast<S&&>(s)});
}
} // namespace tmdesc
