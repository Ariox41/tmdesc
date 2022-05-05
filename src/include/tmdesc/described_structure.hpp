// Copyright Victor Smirnov 2021-2022
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
//
// The documentation can be found at the library's page:
// https://github.com/Ariox41/tmdesc

#pragma once
#include "type_info/type_info_of.hpp"
#include <boost/hana/unpack.hpp>
namespace tmdesc {
struct described_structure_tag {};

namespace detail {

template <class T, class Fn> struct unpack_foldable_struct_impl {
    T owner;
    Fn fn;
    template <class... MI> constexpr auto operator()(const MI&... mi) const {
        return static_cast<Fn&&>(fn)(mi.getter()(static_cast<T>(owner))...);
    }
};
} // namespace detail
} // namespace tmdesc

namespace boost {
namespace hana {
template <class T> struct tag_of<T, when<!is_nothing(::tmdesc::type_members_info_of(type_c<T>))>> {
    using type = ::tmdesc::described_structure_tag;
};
template <> struct unpack_impl<::tmdesc::described_structure_tag> {
    template <typename V, typename Fn> static constexpr auto apply(V&& v, Fn&& fn) {
        return unpack(
            ::tmdesc::detail::members_cache_unchecked<std::decay_t<V>>,
            ::tmdesc::detail::unpack_foldable_struct_impl<V&&, Fn&&>{static_cast<V&&>(v), static_cast<Fn&&>(fn)});
    }
};
} // namespace hana
} // namespace boost
