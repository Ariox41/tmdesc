// Copyright Victor Smirnov 2021
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
//
// The documentation can be found at the library's page:
// https://github.com/Ariox41/tmdesc

#pragma once
#include <type_traits>
namespace tmdesc {
namespace detail {
/// Empty base optimisation
template <class K, class V, bool = ((std::is_empty<V>{}) && (!std::is_final<V>{}))> struct ebo;

// Specialize ebo for empty types
template <class K, class V> struct ebo<K, V, true> : V {
    constexpr ebo()           = default;
    constexpr ebo(ebo&&)      = default;
    constexpr ebo(const ebo&) = default;
    constexpr ebo& operator=(ebo&&) = default;
    constexpr ebo& operator=(const ebo&) = default;

    using V::V;
};

// Specialize ebo for non-empty types
template <class K, class V> struct ebo<K, V, false> {
    constexpr ebo()           = default;
    constexpr ebo(ebo&&)      = default;
    constexpr ebo(const ebo&) = default;
    constexpr ebo& operator=(ebo&&) = default;
    constexpr ebo& operator=(const ebo&) = default;

    template <class T, std::enable_if_t<std::is_constructible<V, T&&>{}, bool> = true>
    explicit constexpr ebo(T&& t)
      : data_(static_cast<T&&>(t)) {}

    V data_;
};

template <class K, class V> constexpr V const& ebo_get(ebo<K, V, true> const& x) noexcept { return x; }

template <class K, class V> constexpr V& ebo_get(ebo<K, V, true>& x) noexcept { return x; }

template <class K, class V> constexpr V&& ebo_get(ebo<K, V, true>&& x) noexcept { return static_cast<V&&>(x); }

template <class K, class V> constexpr V const& ebo_get(ebo<K, V, false> const& x) noexcept { return x.data_; }

template <class K, class V> constexpr V& ebo_get(ebo<K, V, false>& x) noexcept { return x.data_; }

template <class K, class V> constexpr V&& ebo_get(ebo<K, V, false>&& x) noexcept { return static_cast<V&&>(x.data_); }

template <class V, class K> constexpr V const& ebo_get_by_value(ebo<K, V, true> const& x) noexcept { return x; }

template <class V, class K> constexpr V& ebo_get_by_value(ebo<K, V, true>& x) noexcept { return x; }

template <class V, class K> constexpr V&& ebo_get_by_value(ebo<K, V, true>&& x) noexcept { return static_cast<V&&>(x); }

template <class V, class K> constexpr V const& ebo_get_by_value(ebo<K, V, false> const& x) noexcept { return x.data_; }

template <class V, class K> constexpr V& ebo_get_by_value(ebo<K, V, false>& x) noexcept { return x.data_; }

template <class V, class K> constexpr V&& ebo_get_by_value(ebo<K, V, false>&& x) noexcept {
    return static_cast<V&&>(x.data_);
}
} // namespace detail
} // namespace tmdesc