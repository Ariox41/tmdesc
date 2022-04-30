// Copyright Victor Smirnov 2021
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
//
// The documentation can be found at the library's page:
// https://github.com/Ariox41/tmdesc

#pragma once

#include "../concepts/finite_indexable.hpp"
#include "../functional/invoke.hpp"
#include "../functional/ref_obj.hpp"
#include "../meta/logical_operations.hpp"
#include "detail/tuple.hpp"

namespace tmdesc {
/// full constexpr inherit-based tuple without recursive implementation
template <class... Ts> struct tuple final : detail::tuple_storage<std::make_index_sequence<sizeof...(Ts)>, Ts...> {
private:
    using super_t            = detail::tuple_storage<std::make_index_sequence<sizeof...(Ts)>, Ts...>;
    using direct_constructor = typename super_t::direct_constructor;
    struct conversion_constructor {};

public:
    constexpr tuple()             = default;
    constexpr tuple(const tuple&) = default;
    constexpr tuple(tuple&&)      = default;
    constexpr tuple& operator=(const tuple&) = default;
    constexpr tuple& operator=(tuple&&) = default;

    /// Direct initialisation constructor
    /// \note The constructor is not explicit
    template <class... Args,
              std::enable_if_t<meta::recursive_and_v<bool_constant<(sizeof...(Args) >= 1)>,
                                                     bool_constant<(sizeof...(Ts) == sizeof...(Args))>,
                                                     meta::fast_values_and<std::is_constructible<Ts, Args&&>...>>,
                               bool> = true>
    constexpr tuple(Args&&... args) noexcept(meta::fast_values_and_v<std::is_nothrow_constructible<Ts, Args&&>...>)
      : super_t{direct_constructor{}, static_cast<Args&&>(args)...} {}

    /// Converting copy-constructor
    /// \note The constructor is not explicit
    template <class... Us,
              std::enable_if_t<meta::recursive_and_v<bool_constant<(sizeof...(Us) >= 1)>,
                                                     bool_constant<(sizeof...(Ts) == sizeof...(Us))>,
                                                     meta::negation<std::is_same<tuple<Ts...>, tuple<Us...>>>,
                                                     meta::fast_values_and<std::is_constructible<Ts, const Us&>...>>,
                               bool> = true>
    constexpr tuple(const tuple<Us...>& src) noexcept(
        meta::fast_values_and_v<std::is_nothrow_constructible<Ts, const Us&>...>)
      : tuple(src, conversion_constructor{}, std::make_index_sequence<sizeof...(Us)>{}) {}

    /// Converting move-constructor
    /// \note The constructor is not explicit
    template <class... Us,
              std::enable_if_t<meta::recursive_and_v<bool_constant<(sizeof...(Us) >= 1)>,
                                                     bool_constant<(sizeof...(Ts) == sizeof...(Us))>,
                                                     meta::negation<std::is_same<tuple<Ts...>, tuple<Us...>>>,
                                                     meta::fast_values_and<std::is_constructible<Ts, Us&&>...>>,
                               bool> = true>
    constexpr tuple(tuple<Us...>&& src) noexcept(meta::fast_values_and_v<std::is_nothrow_constructible<Ts, Us&&>...>)
      : tuple(std::move(src), conversion_constructor{}, std::make_index_sequence<sizeof...(Us)>{}) {}

    // TODO swap

    /// Converting copy-assignment
    template <class... Us,
              std::enable_if_t<meta::recursive_and_v<bool_constant<(sizeof...(Us) >= 1)>,
                                                     bool_constant<(sizeof...(Ts) == sizeof...(Us))>,
                                                     meta::negation<std::is_same<tuple<Ts...>, tuple<Us...>>>,
                                                     meta::fast_values_and<std::is_assignable<Ts&, const Us&>...>>,
                               bool> = true>
    constexpr tuple& operator=(const tuple<Us...>& src) //
        noexcept(meta::fast_values_and_v<std::is_nothrow_assignable<Ts&, const Us&>...>) {
        assign(src, std::make_index_sequence<sizeof...(Us)>{});
        return *this;
    }

    /// Converting move-assignment
    template <class... Us,
              std::enable_if_t<meta::recursive_and_v<bool_constant<(sizeof...(Us) >= 1)>,
                                                     bool_constant<(sizeof...(Ts) == sizeof...(Us))>,
                                                     meta::negation<std::is_same<tuple<Ts...>, tuple<Us...>>>,
                                                     meta::fast_values_and<std::is_assignable<Ts&, Us&&>...>>,
                               bool> = true>
    constexpr tuple& operator=(tuple<Us...>&& src) //
        noexcept(meta::fast_values_and_v<std::is_nothrow_assignable<Ts&, Us&&>...>) {
        assign(std::move(src), std::make_index_sequence<sizeof...(Us)>{});
        return *this;
    }

private:
    template <class Src, std::size_t... Is>
    constexpr tuple(Src&& src, conversion_constructor, std::index_sequence<Is...>) noexcept(
        meta::fast_values_and_v<
            std::is_nothrow_constructible<Ts, decltype(detail::ebo_get<size_constant<Is>>(std::declval<Src>()))>...>)
      : tuple(detail::ebo_get<size_constant<Is>>(std::forward<Src>(src))...) {}

    template <class Src, std::size_t... Is>
    constexpr void assign(Src&& src, std::index_sequence<Is...>) noexcept(
        meta::fast_values_and_v<
            std::is_nothrow_assignable<Ts, decltype(detail::ebo_get<size_constant<Is>>(std::declval<Src>()))>...>) {
        (void)std::initializer_list<bool>{true, ((void)(detail::ebo_get<size_constant<Is>>(*this) =
                                                            detail::ebo_get<size_constant<Is>>(std::forward<Src>(src))),
                                                 void(), true)...};
    }
};

/// ===============================
///           Finit Indexable
/// ===============================

/// `at` implementation for tuple
template <class... Ts> struct at_impl<tuple<Ts...>> {
    /// v = [v0, v1, ..., vN] => v [index]
    template <std::size_t I, class V> static constexpr decltype(auto) apply(size_constant<I>, V&& v) noexcept {
        return detail::ebo_get<size_constant<I>>(std::forward<V>(v));
    }
};

/// `size` implementation for tuple
template <class... Ts> struct size_impl<tuple<Ts...>> {
    /// v = [v0, v1, ..., vN] => size_c<N + 1>
    template <class V> static constexpr auto apply(V&&) noexcept { return size_c<sizeof...(Ts)>; }
};

/// ===============================
///               Make
/// ===============================

/// @see std::make_tuple
#ifdef TMDESC_DOXYGEN
constexpr auto make_tuple = [](auto&&... args) -> tuple<try_unwrap_ref_obj_type_t<decltype(args)>...> {
    return {std::forward<decltype(args)>(args)...};
};
#else
struct make_tuple_t {
    template <class... Ts>
    constexpr auto operator()(Ts&&... ts) const //
        noexcept(meta::fast_values_and_v<
                 std::is_nothrow_constructible<try_unwrap_ref_obj_type_t<std::decay_t<Ts>>, Ts>...>) {
        return tuple<try_unwrap_ref_obj_type_t<std::decay_t<Ts>>...>{std::forward<Ts>(ts)...};
    }
};
constexpr make_tuple_t make_tuple{};
#endif

/// @see std::tie
#ifdef TMDESC_DOXYGEN
constexpr auto tie = [](auto&... args) -> tuple<decltype(args)&...> { return {args...}; };
#else
struct tie_t {
    template <class... Ts> constexpr tuple<Ts&...> operator()(Ts&... ts) const noexcept { return {ts...}; }
};
constexpr tie_t tie{};
#endif

/// @see std::forward_as_tuple
#ifdef TMDESC_DOXYGEN
constexpr auto forward_as_tuple = [](auto&&... args) -> tuple<decltype(args)&&...> {
    return {std::forward<decltype(args)>(args)...};
};
#else
struct forward_as_tuple_t {
    template <class... Ts> constexpr tuple<Ts&&...> operator()(Ts&&... ts) const noexcept {
        return {std::forward<Ts>(ts)...};
    }
};
constexpr forward_as_tuple_t forward_as_tuple{};
#endif

} // namespace tmdesc
