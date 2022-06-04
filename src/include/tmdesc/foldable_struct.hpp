// Copyright Victor Smirnov 2021-2022
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
//
// The documentation can be found at the library's page:
// https://github.com/Ariox41/tmdesc

#pragma once
#include "get_type_info.hpp"
#include <boost/hana/unpack.hpp>
namespace tmdesc {
struct foldable_struct_tag {};

namespace detail {

template <class T, class Fn> struct unpack_foldable_struct_impl {
    T&& owner;
    Fn&& fn;
    template <class... Ms> constexpr decltype(auto) operator()(const Ms&... ms) const {
        return static_cast<Fn&&>(fn)(ms.accessor()(static_cast<T&&>(owner))...);
    }
};
} // namespace detail
} // namespace tmdesc

namespace boost {
namespace hana {
template <class T> struct tag_of<T, when<is_just(::tmdesc::get_type_members_info(type_c<T>))>> {
    using type = ::tmdesc::foldable_struct_tag;
};
template <> struct unpack_impl<::tmdesc::foldable_struct_tag> {
    template <typename V, typename Fn> static constexpr auto apply(V&& v, Fn&& fn) {
        return unpack(
            ::tmdesc::get_type_members_info(boost::hana::typeid_(v)).value(),
            ::tmdesc::detail::unpack_foldable_struct_impl<V, Fn>{static_cast<V&&>(v), static_cast<Fn&&>(fn)});
    }
};
} // namespace hana
} // namespace boost
