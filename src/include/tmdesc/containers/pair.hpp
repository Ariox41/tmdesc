// Copyright Victor Smirnov 2021
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
//
// The documentation can be found at the library's page:
// https://github.com/Ariox41/tmdesc

#pragma once

#include "../concepts/foldable.hpp"
#include "../concepts/indexable.hpp"
#include "../functional/invoke.hpp"
#include "../functional/ref_obj.hpp"
#include "../meta/logical_operations.hpp"
#include "detail/ebo.hpp"

namespace tmdesc {
/// full constexpr inherit-based pair without recursive implementation
template <class T1, class T2> struct pair final : detail::ebo<size_constant<0>, T1>, detail::ebo<size_constant<1>, T2> {
private:
public:
    constexpr pair()            = default;
    constexpr pair(const pair&) = default;
    constexpr pair(pair&&)      = default;
    constexpr pair& operator=(const pair&) = default;
    constexpr pair& operator=(pair&&) = default;

    /// Direct initialisation constructor
    /// \note The constructor is not explicit
    template <class A1, class A2,
              std::enable_if_t<meta::fast_values_and_v<std::is_constructible<T1, A1>, std::is_constructible<T2, A2>>,
                               bool> = true>
    constexpr pair(A1&& a1, A2&& a2) noexcept(
        meta::fast_values_and_v<std::is_nothrow_constructible<T1, A1>, std::is_nothrow_constructible<T2, A2>>)
      : detail::ebo<size_constant<0>, T1>{std::forward<A1>(a1)}
      , detail::ebo<size_constant<1>, T2>{std::forward<A2>(a2)} {}

    /// Converting copy-constructor
    /// \note The constructor is not explicit
    template <class U1, class U2,
              std::enable_if_t<(!std::is_same<pair<T1, T2>, pair<U1, U2>>{}) && //
                                   (std::is_constructible<T1, const U1&>{}) &&  //
                                   (std::is_constructible<T2, const U2&>{}),
                               bool> = true>
    constexpr pair(const pair<U1, U2>& src) noexcept((std::is_nothrow_constructible<T1, const U1&>{}) &&
                                                     (std::is_nothrow_constructible<T2, const U2&>{}))
      : detail::ebo<size_constant<0>, T1>{detail::ebo_get<size_constant<0>>(src)}
      , detail::ebo<size_constant<1>, T2>{detail::ebo_get<size_constant<1>>(src)} {}

    /// Converting move-constructor
    /// \note The constructor is not explicit
    template <class U1, class U2,
              std::enable_if_t<(!std::is_same<pair<T1, T2>, pair<U1, U2>>{}) && //
                                   (std::is_constructible<T1, U1&&>{}) &&       //
                                   (std::is_constructible<T2, U2&&>{}),
                               bool> = true>
    constexpr pair(const pair<U1, U2>& src) noexcept((std::is_nothrow_constructible<T1, U1&&>{}) &&
                                                     (std::is_nothrow_constructible<T2, U2&&>{}))
      : detail::ebo<size_constant<0>, T1>{detail::ebo_get<size_constant<0>>(std::move(src))}
      , detail::ebo<size_constant<1>, T2>{detail::ebo_get<size_constant<1>>(std::move(src))} {}

    /// TODO noexcept
    constexpr void swap(pair& p1, pair& p2) const {
        using std::swap;
        swap(detail::ebo_get<size_constant<0>>(p1), detail::ebo_get<size_constant<0>>(p2));
        swap(detail::ebo_get<size_constant<1>>(p1), detail::ebo_get<size_constant<1>>(p2));
    }

    /// Converting copy-assignment
    template <class U1, class U2,
              std::enable_if_t<(!std::is_same<pair<T1, T2>, pair<U1, U2>>{}) && //
                                   (std::is_assignable<T1, const U1&>{}) &&     //
                                   (std::is_assignable<T2, const U2&>{}),
                               bool> = true>
    constexpr pair& operator=(const pair<U1, U2>& src) //
        noexcept((std::is_nothrow_assignable<T1, const U1&>{}) && (std::is_nothrow_assignable<T2, const U2&>{})) {
        detail::ebo_get<size_constant<0>>(*this) = detail::ebo_get<size_constant<0>>(src);
        detail::ebo_get<size_constant<1>>(*this) = detail::ebo_get<size_constant<1>>(src);
        return *this;
    }

    /// Converting move-assignment
    template <class U1, class U2,
              std::enable_if_t<(!std::is_same<pair<T1, T2>, pair<U1, U2>>{}) && //
                                   (std::is_assignable<T1, U1&&>{}) &&          //
                                   (std::is_assignable<T2, U2&&>{}),
                               bool> = true>
    constexpr pair& operator=(pair<U1, U2>&& src) //
        noexcept((std::is_nothrow_assignable<T1, U1&&>{}) && (std::is_nothrow_assignable<T2, U2&&>{})) {
        detail::ebo_get<size_constant<0>>(*this) = detail::ebo_get<size_constant<0>>(std::move(src));
        detail::ebo_get<size_constant<1>>(*this) = detail::ebo_get<size_constant<1>>(std::move(src));
        return *this;
    }
};

/// ===============================
///            Indexable
/// ===============================

/// `at` implementation for pair
template <class T1, class T2> struct at_impl<pair<T1, T2>> {
    /// v = [v0, v1, ..., vN] => v [index]
    template <class V> static constexpr decltype(auto) apply(size_constant<0>, V&& v) noexcept {
        return detail::ebo_get<size_constant<0>>(std::forward<V>(v));
    }
    template <class V> static constexpr decltype(auto) apply(size_constant<1>, V&& v) noexcept {
        return detail::ebo_get<size_constant<1>>(std::forward<V>(v));
    }
};

/// `size` implementation for pair
template <class T1, class T2> struct size_impl<pair<T1, T2>> {
    /// v = [v0, v1, ..., vN] => size_c<N + 1>
    template <class V> static constexpr auto apply(V&& v) noexcept { return size_c<2>; }
};

/// ===============================
///             Foldable
/// ===============================

/// `unpack` implementation for pair
template <class T1, class T2> struct unpack_impl<pair<T1, T2>> {

    /// v = [v1, v2, ..., vN] => fn(v1, v2, ..., vN)
    template <class V, class Fn>
    static constexpr auto apply(V&& v, Fn&& fn) noexcept( //
        noexcept(invoke(std::declval<Fn>(), detail::ebo_get<size_constant<0>>(std::declval<V>()),
                        detail::ebo_get<size_constant<1>>(std::declval<V>()))))
        -> decltype(invoke(std::declval<Fn>(), detail::ebo_get<size_constant<0>>(std::declval<V>()),
                           detail::ebo_get<size_constant<1>>(std::declval<V>()))) {
        return invoke(std::forward<Fn>(fn), detail::ebo_get<size_constant<0>>(std::forward<V>(v)),
                      detail::ebo_get<size_constant<1>>(std::forward<V>(v)));
    }
};

/// ===============================
///               Make
/// ===============================

/// @see std::make_pair
#ifdef TMDESC_DOXYGEN
constexpr auto make_pair =
    [](auto&& a1, auto&& a2) -> pair<try_unwrap_ref_obj_type_t<decltype(a1)>, try_unwrap_ref_obj_type_t<decltype(a2)>> {
    return {std::forward<decltype(a1)>(a1), std::forward<decltype(a2)>(a2)};
};
#else
struct make_pair_t {
    template <class T1, class T2>
    constexpr auto operator()(T1&& t1, T2&& t2) const //
        noexcept((std::is_nothrow_constructible<try_unwrap_ref_obj_type_t<std::decay_t<T1>>, T1>{}) &&
                 (std::is_nothrow_constructible<try_unwrap_ref_obj_type_t<std::decay_t<T2>>, T2>{})) {
        return pair<try_unwrap_ref_obj_type_t<std::decay_t<T1>>, try_unwrap_ref_obj_type_t<std::decay_t<T2>>>{
            std::forward<T1>(t1), std::forward<T2>(t2)};
    }
};
constexpr make_pair_t make_pair{};
#endif

} // namespace tmdesc
