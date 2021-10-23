// Copyright Victor Smirnov 2021
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
//
// The documentation can be found at the library's page:
// https://github.com/Ariox41/tmdesc

#pragma once
#include "../../meta/logical_operations.hpp"
#include <type_traits>
#include <utility>
namespace tmdesc {

namespace detail {

template <std::size_t I, class T> struct tuple_item {
    static constexpr std::size_t index = I;
    using type                         = T;
    T value;
};

template <class Indexer, class... Ts> struct tuple_impl;

template <> struct tuple_impl<std::index_sequence<>> {};
template <std::size_t... Is, class... Ts>
struct tuple_impl<std::index_sequence<Is...>, Ts...> : public tuple_item<Is, Ts>... {
protected:
    using self_t = tuple_impl<std::index_sequence<Is...>, Ts...>;

    tuple_impl()                  = default;
    tuple_impl(tuple_impl&&)      = default;
    tuple_impl(const tuple_impl&) = default;
    tuple_impl& operator=(tuple_impl&&) = default;
    tuple_impl& operator=(const tuple_impl&) = default;

public:
    template <
        class... Args,
        std::enable_if_t<meta::conjunction_v<std::is_constructible<Ts, Args>...>, bool> = true>
    constexpr tuple_impl(Args&&... args) noexcept(
        meta::conjunction_v<std::is_nothrow_constructible<Ts, Args&&>...>)
      : tuple_item<Is, Ts>{std::forward<Args>(args)}... {}
};

struct getter_for_tuple_t {
    template <std::size_t I, class T>
    static constexpr const T& get_by_id(const tuple_item<I, T>& e) noexcept {
        return e.value;
    }
    template <std::size_t I, class T> static constexpr T& get_by_id(tuple_item<I, T>& e) noexcept {
        return e.value;
    }
    template <std::size_t I, class T>
    static constexpr T&& get_by_id(tuple_item<I, T>&& e) noexcept {
        return std::move(e).value;
    }
    template <std::size_t I, class T>
    static constexpr T&& get_by_id(const tuple_item<I, T>&& e) noexcept {
        return std::move(e).value;
    }

    template <std::size_t I, class T>
    static detail::tuple_item<I, T> tuple_element_impl(const tuple_item<I, T>&) noexcept;

    template <std::size_t I, class Tuple>
    using tuple_element = decltype(tuple_element_impl<I>(std::declval<const Tuple&>()));
};

struct getter_by_type_for_tuple_t {
    template <class T, std::size_t I>
    static constexpr const T& get_by_type(const tuple_item<I, T>& e) noexcept {
        return e.value;
    }
    template <class T, std::size_t I>
    static constexpr T& get_by_type(tuple_item<I, T>& e) noexcept {
        return e.value;
    }
    template <class T, std::size_t I>
    static constexpr T&& get_by_type(tuple_item<I, T>&& e) noexcept {
        return std::move(e).value;
    }
    template <class T, std::size_t I>
    static constexpr T&& get_by_type(const tuple_item<I, T>&& e) noexcept {
        return std::move(e).value;
    }
};

} // namespace detail
} // namespace tmdesc
