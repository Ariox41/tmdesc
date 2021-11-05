// Copyright Victor Smirnov 2021
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
//
// The documentation can be found at the library's page:
// https://github.com/Ariox41/tmdesc

#pragma once
#include "../../meta/logical_operations.hpp"
#include "../../meta/type_t.hpp"
#include <type_traits>
#include <utility>
namespace tmdesc {

namespace detail {

template <std::size_t I, class T> struct tuple_item {
    static constexpr std::size_t index = I;
    using type                         = T;
    T value;
};

struct getter_by_id_for_tuple_t {
    template <std::size_t I, class T> //
    static constexpr const T& get_by_id(const tuple_item<I, T>& e) noexcept {
        return e.value;
    }
    template <std::size_t I, class T> //
    static constexpr T& get_by_id(tuple_item<I, T>& e) noexcept {
        return e.value;
    }
    template <std::size_t I, class T> //
    static constexpr T&& get_by_id(tuple_item<I, T>&& e) noexcept {
        return std::move(e).value;
    }
    template <std::size_t I, class T> //
    static constexpr const T&& get_by_id(const tuple_item<I, T>&& e) noexcept {
        return std::move(e).value;
    }

    template <std::size_t I, class T>
    static constexpr tuple_item<I, T> tuple_element_impl(const tuple_item<I, T>&) noexcept;

    template <std::size_t I, class Tuple>
    using tuple_element = decltype(tuple_element_impl<I>(std::declval<const Tuple&>()));
};

template <class Indexer, class... Ts> struct tuple_impl;

template <> struct tuple_impl<std::index_sequence<>> {};

template <std::size_t... Is, class... Ts>
struct tuple_impl<std::index_sequence<Is...>, Ts...> : public tuple_item<Is, Ts>... {
protected:
    using self_t = tuple_impl<std::index_sequence<Is...>, Ts...>;
    using getter = getter_by_id_for_tuple_t;

    constexpr tuple_impl()                  = default;
    constexpr tuple_impl(tuple_impl&&)      = default;
    constexpr tuple_impl(const tuple_impl&) = default;
    constexpr tuple_impl& operator=(tuple_impl&&) = default;
    constexpr tuple_impl& operator=(const tuple_impl&) = default;

public:
    template <class... Args, std::enable_if_t<meta::conjunction_v<std::is_constructible<Ts, Args>...>, bool> = true>
    constexpr tuple_impl(Args&&... args) noexcept(meta::conjunction_v<std::is_nothrow_constructible<Ts, Args&&>...>)
      : tuple_item<Is, Ts>{std::forward<Args>(args)}... {}

    // Converting copy-constructor. see std::tuple.
    template <class... Us,
              std::enable_if_t<
                  meta::conjunction_v<meta::negation<std::is_same<meta::type_list<Ts...>, meta::type_list<Us...>>>,
                                      std::is_constructible<Ts, const Us&>...>,
                  bool> = true>
    constexpr tuple_impl(const tuple_impl<std::index_sequence<Is...>, Us...>& src) noexcept(
        meta::conjunction_v<std::is_nothrow_constructible<Ts, const Us&>...>)
      : tuple_item<Is, Ts>{getter::get_by_id<Is>(src)}... {}

    // Converting move-constructor. see std::tuple.
    template <class... Us,
              std::enable_if_t<
                  meta::conjunction_v<meta::negation<std::is_same<meta::type_list<Ts...>, meta::type_list<Us...>>>,
                                      std::is_constructible<Ts, Us&&>...>,
                  bool> = true>
    constexpr tuple_impl(tuple_impl<std::index_sequence<Is...>, Us...>&& src) noexcept(
        meta::conjunction_v<std::is_nothrow_constructible<Ts, Us&&>...>)
      : tuple_item<Is, Ts>{std::forward<Us>(getter::get_by_id<Is>(src))}... {}

    //    // Converting copy-constructor. see std::tuple.
    //    template <class... Us,
    //              std::enable_if_t<
    //                  meta::conjunction_v<meta::negation<std::is_same<meta::type_list<Ts...>,
    //                  meta::type_list<Us...>>>,
    //                                      std::is_constructible<Ts, const Us&>..., std::is_convertible<const Us&,
    //                                      Ts>...>,
    //                  bool> = true>
    //    constexpr tuple_impl(const tuple_impl<std::index_sequence<Is...>, Us...>& src) noexcept(
    //        meta::conjunction_v<std::is_nothrow_constructible<Ts, const Us&>...>)
    //      : tuple_item<Is, Ts>{getter::get_by_id<Is>(src)}... {}

    //    // Explicit converting copy-constructor. see std::tuple.
    //    template <class... Us,
    //              std::enable_if_t<
    //                  meta::conjunction_v<meta::negation<std::is_same<meta::type_list<Ts...>,
    //                  meta::type_list<Us...>>>,
    //                                      std::is_constructible<Ts, const Us&>...,
    //                                      meta::negation<meta::conjunction<std::is_convertible<const Us&, Ts>...>>>,
    //                  bool> = true>
    //    explicit constexpr tuple_impl(const tuple_impl<std::index_sequence<Is...>, Us...>& src) noexcept(
    //        meta::conjunction_v<std::is_nothrow_constructible<Ts, const Us&>...>)
    //      : tuple_item<Is, Ts>{getter::get_by_id<Is>(src)}... {}

    //    // Converting move-constructor. see std::tuple.
    //    template <class... Us,
    //              std::enable_if_t<
    //                  meta::conjunction_v<meta::negation<std::is_same<meta::type_list<Ts...>,
    //                  meta::type_list<Us...>>>,
    //                                      std::is_constructible<Ts, Us&&>..., std::is_convertible<Us&&, Ts>...>,
    //                  bool> = true>
    //    constexpr tuple_impl(tuple_impl<std::index_sequence<Is...>, Us...>&& src) noexcept(
    //        meta::conjunction_v<std::is_nothrow_constructible<Ts, Us&&>...>)
    //      : tuple_item<Is, Ts>{std::forward<Us>(getter::get_by_id<Is>(src))}... {}

    //    // Explicit converting move-constructor. see std::tuple.
    //    template <class... Us,
    //              std::enable_if_t<
    //                  meta::conjunction_v<meta::negation<std::is_same<meta::type_list<Ts...>,
    //                  meta::type_list<Us...>>>,
    //                                      std::is_constructible<Ts, Us&&>...,
    //                                      meta::negation<meta::conjunction<std::is_convertible<Us&&, Ts>...>>>,
    //                  bool> = true>
    //    explicit constexpr tuple_impl(tuple_impl<std::index_sequence<Is...>, Us...>&& src) noexcept(
    //        meta::conjunction_v<std::is_nothrow_constructible<Ts, Us&&>...>)
    //      : tuple_item<Is, Ts>{std::forward<Us>(getter::get_by_id<Is>(src))}... {}
};

struct getter_by_type_for_tuple_t {
    template <class T, std::size_t I> //
    static constexpr const T& get_by_type(const tuple_item<I, T>& e) noexcept {
        return e.value;
    }
    template <class T, std::size_t I> //
    static constexpr T& get_by_type(tuple_item<I, T>& e) noexcept {
        return e.value;
    }
    template <class T, std::size_t I> //
    static constexpr T&& get_by_type(tuple_item<I, T>&& e) noexcept {
        return std::move(e).value;
    }
    template <class T, std::size_t I> //
    static constexpr const T&& get_by_type(const tuple_item<I, T>&& e) noexcept {
        return std::move(e).value;
    }
};

} // namespace detail
} // namespace tmdesc
