// Copyright Victor Smirnov 2021-2022
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
//
// The documentation can be found at the library's page:
// https://github.com/Ariox41/tmdesc

#pragma once
#include <boost/hana/fwd/at.hpp>
#include <boost/hana/fwd/equal.hpp>
#include <boost/hana/fwd/find_if.hpp>
#include <boost/hana/fwd/fold_left.hpp>
#include <boost/hana/fwd/fold_right.hpp>
#include <boost/hana/fwd/length.hpp>
#include <boost/hana/fwd/string.hpp>
#include <boost/hana/fwd/unpack.hpp>
#include <boost/hana/integral_constant.hpp>
#include <boost/hana/range.hpp>
#include <tmdesc/functional/invoke.hpp>
#include <tmdesc/get_type_info.hpp>
namespace tmdesc {

template <typename T>
struct FoldableStruct : hana::bool_<(hana::is_just(tmdesc::get_type_members_info(hana::type_c<T>)))> {};

namespace detail {

template <typename S, typename Fn> struct unpack_helper_ {
    S&& struct_;
    Fn&& fn_;
    template <class... AC, class... AS>
    constexpr auto operator()(const tmdesc::member_info<AC, AS>&... ms) noexcept(
        noexcept(tmdesc::invoke(std::declval<Fn>(), tmdesc::invoke(ms.accessor(), std::declval<S>())...)))
        -> decltype(tmdesc::invoke(std::declval<Fn>(), tmdesc::invoke(ms.accessor(), std::declval<S>())...)) {
        return tmdesc::invoke(static_cast<Fn&&>(fn_), tmdesc::invoke(ms.accessor(), static_cast<S&&>(struct_))...);
    }
};

template <typename S1, typename S2> struct member_equal_helper {
    const S1& s1;
    const S2& s2;

    template <class M> constexpr auto operator()(const M& m) const {
        return hana::equal(tmdesc::invoke(m.accessor(), s1), tmdesc::invoke(m.accessor(), s2));
    }
};

template <typename S, typename Fn> struct find_if_helper_ {
    S&& struct_;
    Fn& fn;
    template <typename M>
    constexpr auto operator()(const M& mi) const
        noexcept(noexcept(tmdesc::invoke(std::declval<Fn&>(), tmdesc::invoke(mi.accessor(), std::declval<S&&>()))))
            -> decltype(tmdesc::invoke(std::declval<Fn&>(), tmdesc::invoke(mi.accessor(), std::declval<S&&>()))) {
        return tmdesc::invoke(fn, tmdesc::invoke(mi.accessor(), static_cast<S&&>(struct_)));
    }
};

} // namespace detail
} // namespace tmdesc

BOOST_HANA_NAMESPACE_BEGIN

template <typename T> struct unpack_impl<T, when<::tmdesc::FoldableStruct<T>::value>> {
    template <typename V, typename Fn> //
    static constexpr decltype(auto) apply(V&& v, Fn&& fn) {
        return hana::unpack(*::tmdesc::get_type_members_info(typeid_(v)),
                            ::tmdesc::detail::unpack_helper_<V, Fn>{static_cast<V&&>(v), static_cast<Fn&&>(fn)});
    }
};

template <typename T> struct length_impl<T, when<::tmdesc::FoldableStruct<T>::value>> {
    template <typename Xs> //
    static constexpr auto apply(Xs const&) {
        return hana::length(*::tmdesc::get_type_members_info(type_c<T>));
    }
};

template <typename T> struct equal_impl<T, T, when<::tmdesc::FoldableStruct<T>::value>> {
    template <typename V1, typename V2> //
    static constexpr auto apply(V1 const& v1, V2 const& v2) {
        return hana::all_of(*::tmdesc::get_type_members_info(type_c<V1>),
                            ::tmdesc::detail::member_equal_helper<V1, V2>{v1, v2});
    }
};

template <typename S> struct find_if_impl<S, when<::tmdesc::FoldableStruct<S>::value>> {
    template <typename X, typename Pred> static constexpr decltype(auto) apply(X&& x, Pred&& pred) {
        return hana::find_if(make_range(size_c<0>, length(*::tmdesc::get_type_members_info(typeid_(x)))),
                             ::tmdesc::detail::find_if_helper_<X, Pred>{static_cast<X&&>(x), pred});
    }
};

BOOST_HANA_NAMESPACE_END
