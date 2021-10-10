// Copyright Victor Smirnov 2021
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)

#pragma once

#include "../meta/helpers.hpp"
#include "../type_description.hpp"
namespace tmdesc {
/// stored flag set with tags for access
template <class... Flags> struct flag_map;

template <class... Tags, class... Flags>
struct flag_map<::tmdesc::flag<Tags, Flags>...> : public ::tmdesc::flag<Tags, Flags>... {
    constexpr flag_map(::tmdesc::flag<Tags, Flags>... flags) noexcept
      : flag<Tags, Flags>{flags}... {}

    template <class Tag> constexpr auto find_flag() const noexcept { return find_flag_impl<Tag>(this); }

private:
    template <class Tag, class Flag>
    static constexpr meta::some<Flag> find_flag_impl(const ::tmdesc::flag<Tag, Flag>* f) noexcept {
        return {*f};
    }
    template <class Tag> static constexpr meta::none find_flag_impl(const void*) noexcept { return {}; }

    static_assert(meta::fast_and({flag_map<::tmdesc::flag<Tags, Flags>...>::find_flag_impl<Tags>(
                                      static_cast<flag_map<::tmdesc::flag<Tags, Flags>...>*>(0))
                                      .is_some()...}),
                  "tag duplication");
};

} // namespace tmdesc
