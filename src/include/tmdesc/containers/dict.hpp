// Copyright Victor Smirnov 2021
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
//
// The documentation can be found at the library's page:
// https://github.com/Ariox41/tmdesc

#pragma once
#include "../../meta/integral_constant.hpp"
#include "../../meta/logical_operations.hpp"
#include "ebo.hpp"
namespace tmdesc {
namespace detail {
template <class Indexer, class... Ts> struct tuple_storage;

template <std::size_t... Is, class... Ts>
struct tuple_storage<std::index_sequence<Is...>, Ts...> : public detail::ebo<size_constant<Is>, Ts>... {
protected:
    constexpr tuple_storage()                     = default;
    constexpr tuple_storage(tuple_storage&&)      = default;
    constexpr tuple_storage(const tuple_storage&) = default;
    constexpr tuple_storage& operator=(tuple_storage&&) = default;
    constexpr tuple_storage& operator=(const tuple_storage&) = default;

    struct direct_constructor {};

    template <class... Args>
    constexpr tuple_storage(direct_constructor, Args&&... args) noexcept(
        meta::fast_values_and_v<std::is_nothrow_constructible<Ts, Args&&>...>)
      : detail::ebo<size_constant<Is>, Ts>{std::forward<Args>(args)}... {}
};

} // namespace detail
} // namespace tmdesc
