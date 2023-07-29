// Copyright Victor Smirnov 2021-2022
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
//
// The documentation can be found at the library's page:
// https://github.com/Ariox41/tmdesc

#pragma once
#include "get_type_info.hpp"
#include <boost/hana/at.hpp>
#include <boost/hana/equal.hpp>
#include <boost/hana/find_if.hpp>
#include <boost/hana/fwd/fold_left.hpp>
#include <boost/hana/fwd/fold_right.hpp>
#include <boost/hana/integral_constant.hpp>
#include <boost/hana/length.hpp>
#include <boost/hana/range.hpp>
#include <boost/hana/string.hpp>
#include <boost/hana/unpack.hpp>
namespace tmdesc {

template <typename T>
struct FoldableStruct : boost::hana::bool_<(boost::hana::is_just(get_type_members_info(boost::hana::type_c<T>)))> {};

/// Reference to a member of the object
template <class S, std::size_t I> struct member_ref {
    explicit constexpr member_ref(S&& struct_ref)
      : struct_ref_(static_cast<S&&>(struct_ref)) {}
    static constexpr zstring_view name() noexcept {
        return boost::hana::at_c<I>(get_type_members_info(boost::hana::type_c<S>).value()).name();
    }
    static constexpr auto type() noexcept {
        return boost::hana::at_c<I>(get_type_members_info(boost::hana::type_c<S>).value()).type();
    }
    static constexpr decltype(auto) attributes() noexcept {
        return boost::hana::at_c<I>(get_type_members_info(boost::hana::type_c<S>).value()).attributes();
    }
    constexpr decltype(auto) value() const noexcept {
        return get_type_members_info(boost::hana::type_c<S>).value().accessor()(static_cast<S&&>(struct_ref_));
    }

private:
    S&& struct_ref_;
};

namespace detail {

template <typename S, typename Fn> struct unpack_helper_ {
    S&& struct_;
    Fn&& fn;
    template <class... I> constexpr decltype(auto) operator()(I...) const {
        return static_cast<Fn&&>(fn)(::tmdesc::member_ref<S, I::value>{static_cast<S&&>(struct_)}...);
    }
};
template <typename S, typename Fn> struct fold_left_helper_ {
    S&& struct_;
    Fn& fn;
    template <typename State, typename I> constexpr decltype(auto) operator()(State&& state, I) const {
        return fn(static_cast<S&&>(state), ::tmdesc::member_ref<S, I::value>{static_cast<S&&>(struct_)});
    }
};
template <typename S, typename Fn> struct fold_right_helper_ {
    S&& struct_;
    Fn& fn;
    template <typename State, typename I> constexpr decltype(auto) operator()(I, State&& state) const {
        return fn(::tmdesc::member_ref<S, I::value>{static_cast<S&&>(struct_)}, static_cast<S&&>(state));
    }
};

template <typename S1, typename S2> struct member_equal_helper {
    const S1& s1;
    const S2& s2;

    template <class M> constexpr auto operator()(const M& m) const {
        return hana::equal(m.accessor()(s1), m.accessor()(s2));
    }
};

template <typename S, typename Fn> struct find_if_helper_ {
    S&& struct_;
    Fn& fn;
    template <typename I> constexpr decltype(auto) operator()(I) const {
        return fn(::tmdesc::member_ref<S, I::value>{static_cast<S&&>(struct_)});
    }
};

} // namespace detail
} // namespace tmdesc

BOOST_HANA_NAMESPACE_BEGIN

template <typename T> struct unpack_impl<T, when<::tmdesc::FoldableStruct<T>::value>> {
    template <typename V, typename Fn> //
    static constexpr decltype(auto) apply(V&& v, Fn&& fn) {
        return unpack(make_range(size_c<0>, length(*::tmdesc::get_type_members_info(typeid_(v)))),
                      ::tmdesc::detail::unpack_helper_<V, Fn>{static_cast<V&&>(v), static_cast<Fn&&>(fn)});
    }
};
template <typename T> struct fold_left_impl<T, when<::tmdesc::FoldableStruct<T>::value>> {
    template <typename V, typename Fn> //
    static constexpr decltype(auto) apply(V&& v, Fn&& fn) {
        return fold_left(make_range(size_c<0>, length(*::tmdesc::get_type_members_info(typeid_(v)))),
                         ::tmdesc::detail::fold_left_helper_<V, Fn>{static_cast<V&&>(v), static_cast<Fn&&>(fn)});
    }
    template <typename V, typename S, typename Fn> //
    static constexpr decltype(auto) apply(V&& v, S&& s, Fn&& fn) {
        return fold_left(make_range(size_c<0>, length(*::tmdesc::get_type_members_info(typeid_(v)))),
                         static_cast<S&&>(s),
                         ::tmdesc::detail::fold_left_helper_<V, Fn>{static_cast<V&&>(v), static_cast<Fn&&>(fn)});
    }
};
template <typename T> struct fold_right_impl<T, when<::tmdesc::FoldableStruct<T>::value>> {
    template <typename V, typename Fn> //
    static constexpr decltype(auto) apply(V&& v, Fn&& fn) {
        return fold_right(make_range(size_c<0>, length(*::tmdesc::get_type_members_info(typeid_(v)))),
                          ::tmdesc::detail::fold_right_helper_<V, Fn>{static_cast<V&&>(v), static_cast<Fn&&>(fn)});
    }
    template <typename V, typename S, typename Fn> //
    static constexpr decltype(auto) apply(V&& v, S&& s, Fn&& fn) {
        return fold_right(make_range(size_c<0>, length(*::tmdesc::get_type_members_info(typeid_(v)))),
                          static_cast<S&&>(s),
                          ::tmdesc::detail::fold_right_helper_<V, Fn>{static_cast<V&&>(v), static_cast<Fn&&>(fn)});
    }
};

template <typename T> struct length_impl<T, when<::tmdesc::FoldableStruct<T>::value>> {
    template <typename Xs> //
    static constexpr auto apply(Xs const&) {
        return length(*::tmdesc::get_type_members_info(type_c<T>));
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
