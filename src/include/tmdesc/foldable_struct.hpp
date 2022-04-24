// Copyright Victor Smirnov 2021
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
//
// The documentation can be found at the library's page:
// https://github.com/Ariox41/tmdesc

#pragma once
#include "type_info/get_type_info.hpp"
#include <boost/hana/fwd/unpack.hpp>
namespace tmdesc {
namespace tags {
struct foldable_struct_tag {};

} // namespace tags

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
template <class T> struct tag_of<T, when<!is_nothing(::tmdesc::static_type_members_v<T>)>> {
    using type = ::tmdesc::tags::foldable_struct_tag;
};
template <> struct unpack_impl<::tmdesc::tags::foldable_struct_tag> {
    template <typename Xs, typename F> static constexpr auto apply(Xs&& xs, F&& f) {
        return unpack(
            ::tmdesc::static_type_members_v<std::decay_t<Xs>>,
            ::tmdesc::detail::unpack_foldable_struct_impl<Xs&&, F&&>{static_cast<Xs&&>(xs), static_cast<F&&>(f)});
    }
};
} // namespace hana
} // namespace boost
