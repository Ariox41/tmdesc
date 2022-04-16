// Copyright Victor Smirnov 2021
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
//
// The documentation can be found at the library's page:
// https://github.com/Ariox41/tmdesc

#pragma once

#include "meta/logical_operations.hpp"
#include "functional/optional.hpp"
#include "meta/type_traits.hpp"
#include "type_description.hpp"
namespace tmdesc {
/** stored flag set with tags for access

    @tparam Flags - List of @ref flag
*/
template <class... Flags> struct flag_map;

template <class... Tags, class... Flags>
struct flag_map<::tmdesc::flag<Tags, Flags>...> : public ::tmdesc::flag<Tags, Flags>... {
    constexpr flag_map()                = default;
    constexpr flag_map(flag_map&&)      = default;
    constexpr flag_map(const flag_map&) = default;

    constexpr flag_map& operator=(flag_map&&) = delete;
    constexpr flag_map& operator=(const flag_map&) = delete;

    constexpr flag_map(::tmdesc::flag<Tags, Flags>... flags) noexcept
      : ::tmdesc::flag<Tags, Flags>{flags}... {
        static_assert(
            meta::fast_and_v<is_some<std::decay_t<decltype(std::declval<flag_map<::tmdesc::flag<Tags, Flags>...>>().find_flag(
                type_t<Tags>{}))>>::value...>,
            "tag duplication detected");
    }

    /** find flag value for flag tag

        @tparam Tag
        A tag type used as a key to find the value.
        The `Tag` can be passed as a template argument or as a value argumnet of type_t<Tag>

        @return @ref meta::some with flag value if the flag is found,
        or @ref meta::none if not found
     */
    template <class Tag> constexpr auto find_flag(type_t<Tag> = {}) const noexcept {
        return find_flag_impl<Tag>(this);
    }

private:
    // return some<reference> because `flag_map` must have a static lifetime
    template <class Tag, class Flag>
    static constexpr some_t<const Flag&> find_flag_impl(const ::tmdesc::flag<Tag, Flag>* f) noexcept {
        return {f->value};
    }
    template <class Tag> static constexpr none_t find_flag_impl(const void*) noexcept { return {}; }
};

template <> struct flag_map<> {
    template <class Tag> constexpr none_t find_flag(type_t<Tag> = {}) const noexcept { return {}; }
};

} // namespace tmdesc
