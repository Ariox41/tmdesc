// Copyright Victor Smirnov 2021
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
//
// The documentation can be found at the library's page:
// https://github.com/Ariox41/tmdesc

#pragma once

#include "meta/logical_operations.hpp"
#include "meta/optional_type.hpp"
#include "meta/type_traits.hpp"
#include "type_description.hpp"
namespace tmdesc {
/** stored flag set with tags for access

    @tparam Flags - List of @ref flag
*/
template <class... Flags> struct flag_map;

template <class... Tags, class... Flags>
struct flag_map<::tmdesc::flag<Tags, Flags>...> : public ::tmdesc::flag<Tags, Flags>... {
    constexpr flag_map() = default;
    constexpr flag_map(flag_map&&) = default;
    constexpr flag_map(const flag_map&) = default;

    constexpr flag_map& operator=(flag_map&&) = delete;
    constexpr flag_map& operator=(const flag_map&) = delete;

    constexpr flag_map(::tmdesc::flag<Tags, Flags>... flags) noexcept
      : ::tmdesc::flag<Tags, Flags>{flags}... {
        static_assert(
            meta::fast_and({meta::remove_cvref_t<
                decltype(std::declval<flag_map<::tmdesc::flag<Tags, Flags>...>>().find_flag(
                    type_t<Tags>{}))>::is_some()...}),
            "tag duplication detected");
    }

    /** find flag value for flag tag

        @tparam Tag
        A tag type used as a key to find the value.
        The `Tag` can be passed as a template argument or as a value argumnet of type_t<Tag>

        @return @ref meta::some with flag value if the flag is found,
        or @ref meta::none if not found
     */
    template <class Tag> constexpr auto find_flag(type_t<Tag> tag = {}) const noexcept {
        return find_flag_impl(tag, this);
    }

private:
    // return some<reference> because `flag_map` must have a static lifetime
    template <class Tag, class Flag>
    static constexpr meta::some<const Flag&>
    find_flag_impl(type_t<Tag>, const ::tmdesc::flag<Tag, Flag>* f) noexcept {
        return {f->value};
    }
    template <class Tag>
    static constexpr meta::none find_flag_impl(type_t<Tag>, const void*) noexcept {
        return {};
    }
};

template <> struct flag_map<> {
    template <class Tag> constexpr meta::none find_flag(type_t<Tag> = {}) const noexcept {
        return {};
    }
};

} // namespace tmdesc
