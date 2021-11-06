// Copyright Victor Smirnov 2021
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
//
// The documentation can be found at the library's page:
// https://github.com/Ariox41/tmdesc

#pragma once

#include "../functional/reference_wrapper_helpers.hpp"
#include "../get.hpp"
#include "detail/tuple.hpp"
#include <utility>

namespace tmdesc {
/// full constexpr inherit-based tuple without recursive implementation
template <class... Ts> struct tuple final : detail::tuple_impl<std::make_index_sequence<sizeof...(Ts)>, Ts...> {
private:
    using super_t           = detail::tuple_impl<std::make_index_sequence<sizeof...(Ts)>, Ts...>;
    using direct_constuctor = typename super_t::direct_constuctor;
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
              std::enable_if_t<meta::recursive_and_v<meta::bool_constant<(sizeof...(Args) >= 1)>,
                                                     meta::bool_constant<(sizeof...(Ts) == sizeof...(Args))>,
                                                     meta::fast_values_and<std::is_constructible<Ts, Args&&>...>>,
                               bool> = true>
    constexpr tuple(Args&&... args) noexcept(meta::fast_values_and_v<std::is_nothrow_constructible<Ts, Args&&>...>)
      : super_t{direct_constuctor{}, static_cast<Args&&>(args)...} {}

    /// Converting copy-constructor
    /// \note The constructor is not explicit
    template <class... Us,
              std::enable_if_t<meta::recursive_and_v<meta::bool_constant<(sizeof...(Us) >= 1)>,
                                                     meta::bool_constant<(sizeof...(Ts) == sizeof...(Us))>,
                                                     meta::negation<std::is_same<tuple<Ts...>, tuple<Us...>>>,
                                                     meta::fast_values_and<std::is_constructible<Ts, const Us&>...>>,
                               bool> = true>
    constexpr tuple(const tuple<Us...>& src) noexcept(
        meta::fast_values_and_v<std::is_nothrow_constructible<Ts, const Us&>...>)
      : tuple(src, conversion_constructor{}, std::make_index_sequence<sizeof...(Us)>{}) {}

    /// Converting move-constructor
    /// \note The constructor is not explicit
    template <class... Us,
              std::enable_if_t<meta::recursive_and_v<meta::bool_constant<(sizeof...(Us) >= 1)>,
                                                     meta::bool_constant<(sizeof...(Ts) == sizeof...(Us))>,
                                                     meta::negation<std::is_same<tuple<Ts...>, tuple<Us...>>>,
                                                     meta::fast_values_and<std::is_constructible<Ts, Us&&>...>>,
                               bool> = true>
    constexpr tuple(tuple<Us...>&& src) noexcept(meta::fast_values_and_v<std::is_nothrow_constructible<Ts, Us&&>...>)
      : tuple(std::move(src), conversion_constructor{}, std::make_index_sequence<sizeof...(Us)>{}) {}

    // TODO swap

    /// Converting copy-assigment
    template <class... Us,
              std::enable_if_t<meta::recursive_and_v<meta::bool_constant<(sizeof...(Ts) == sizeof...(Us))>,
                                                     meta::negation<std::is_same<tuple<Ts...>, tuple<Us...>>>,
                                                     meta::fast_values_and<std::is_assignable<Ts&, const Us&>...>>,
                               bool> = true>
    constexpr tuple& operator=(const tuple<Us...>& src) //
        noexcept(meta::fast_values_and_v<std::is_nothrow_assignable<Ts&, const Us&>...>) {
        assign(src, std::make_index_sequence<sizeof...(Us)>{});
        return *this;
    }

    /// Converting move-assigment
    template <class... Us,
              std::enable_if_t<meta::recursive_and_v<meta::bool_constant<(sizeof...(Ts) == sizeof...(Us))>,
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
        meta::fast_values_and_v<std::is_nothrow_constructible<Ts, tuple_get_result_t<Is, Src>>...>)
      : tuple{get<Is>(src)...} {}

    template <class Src, std::size_t... Is>
    constexpr void assign(Src&& src, std::index_sequence<Is...>) noexcept(
        meta::fast_values_and_v<std::is_nothrow_assignable<Ts, tuple_get_result_t<Is, Src>>...>) {
        (void)std::initializer_list<bool>{true,
                                          ((void)(get<Is>(*this) = get<Is>(std::forward<Src>(src))), void(), true)...};
    }
};

template <class T> struct is_tuple : std::false_type {};
template <class... Ts> struct is_tuple<tuple<Ts...>> : std::true_type {};
template <class T> constexpr bool is_tuple_v = is_tuple<T>::value;

template <class... Ts>
struct tuple_size<tuple<Ts...>, std::enable_if_t<true>> : public std::integral_constant<std::size_t, sizeof...(Ts)> {};

struct make_tuple_t {
    template <class... Ts>
    constexpr tuple<unwrap_ref_decay_t<Ts>...> operator()(Ts&&... ts) const
        noexcept(meta::fast_values_and_v<std::is_nothrow_constructible<unwrap_ref_decay_t<Ts>, Ts>...>) {
        return tuple<unwrap_ref_decay_t<Ts>...>{std::forward<Ts>(ts)...};
    }
};
constexpr make_tuple_t make_tuple{};

struct tie_t {
    template <class... Ts> constexpr tuple<Ts&...> operator()(Ts&... ts) const noexcept { return {ts...}; }
};
constexpr tie_t tie{};

struct forward_as_tuple_t {
    template <class... Ts> constexpr tuple<Ts&&...> operator()(Ts&&... ts) const noexcept {
        return {std::forward<Ts>(ts)...};
    }
};
constexpr forward_as_tuple_t forward_as_tuple{};

template <class... Ts> struct tuple_getter<tuple<Ts...>> { using type = detail::getter_by_id_for_tuple_t; };
template <class... Ts> struct tuple_getter_by_type<tuple<Ts...>> { using type = detail::getter_by_type_for_tuple_t; };
} // namespace tmdesc
