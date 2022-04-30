// Copyright Victor Smirnov 2021
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
//
// The documentation can be found at the library's page:
// https://github.com/Ariox41/tmdesc
#pragma once
#include "../concepts/foldable.hpp"
#include "../functional/invoke.hpp"
#include <type_traits>
namespace tmdesc {

/// Similar to std::optional, tmdesc::optional represents a value of some type or a non-existent value.
/// Unlike std::optional, the optional-ness is known at compile time.
/// @see some
/// @see none
/// @see is_some
/// @see is_none
/// @see eval_if
template <class... T> struct optional;

template <class T> struct optional<T> {
    constexpr optional()                = default;
    constexpr optional(optional&&)      = default;
    constexpr optional(const optional&) = default;
    constexpr optional& operator=(optional&&) = default;
    constexpr optional& operator=(const optional&) = default;

    template <class U, std::enable_if_t<std::is_constructible<T, U>::value, bool> = true>
    constexpr optional(U&& value) noexcept(std::is_nothrow_constructible<T, U&&>::value)
      : value_{std::forward<U>(value)} {}

    constexpr const T& value() const& noexcept { return value_; }
    constexpr const T& value() const&& noexcept { return std::move(value_); }
    constexpr T& value() & noexcept { return value_; }
    constexpr T&& value() && noexcept { return static_cast<T&&>(value_); }

private:
    T value_;
};
// TODO do we need it?
template <class T> struct optional<T&> {
    constexpr optional(T& ref)
      : ref_(ref) {}
    constexpr optional(const optional&) = default;
    constexpr optional& operator=(const optional&) = default;

    constexpr T& value() const { return ref_; }

private:
    T& ref_;
};

template <> struct optional<> {
    constexpr optional()                = default;
    constexpr optional(optional&&)      = default;
    constexpr optional(const optional&) = default;
    constexpr optional& operator=(optional&&) = default;
    constexpr optional& operator=(const optional&) = default;

    // optional<>  does not contain a value
    constexpr auto value() const = delete;
};

template <class T> using some_t = optional<T>;
using none_t                    = optional<>;

template <class T>
some_t<std::decay_t<T>> some(T&& v) noexcept(std::is_nothrow_constructible<std::decay_t<T>, T&&>::value) {
    return some_t<std::decay_t<T>>{static_cast<T&&>(v)};
}

constexpr none_t none{};

template <class T> constexpr true_type is_some(const some_t<T>&) noexcept { return {}; }
constexpr false_type is_some(none_t) noexcept { return {}; }

template <class T> constexpr false_type is_none(const some_t<T>&) noexcept { return {}; }
constexpr true_type is_none(none_t) noexcept { return {}; }

template <class T, class U>
constexpr auto operator==(const optional<T>& lha, const optional<U>& rha) noexcept(noexcept(std::declval<const T&>() ==
                                                                                            std::declval<const U&>()))
    -> decltype(std::declval<const T&>() == std::declval<const U&>()) {
    return lha.value() == rha.value();
}
template <class T> constexpr bool operator==(const optional<T>&, const optional<>&) noexcept { return false; }
template <class T> constexpr bool operator==(const optional<>&, const optional<T>&) noexcept { return false; }
constexpr bool operator==(const optional<>&, const optional<>&) noexcept { return true; }

/// =======================
///         Foldable
/// =======================

/// `unpack` implementation for optional
template <class T> struct unpack_impl<some_t<T>> {
    /// v = [v1, v2, ..., vN] => fn(v1, v2, ..., vN)
    template <class V, class Fn>
    static constexpr auto apply(V&& v,
                                Fn&& fn) //
        noexcept(noexcept(invoke(std::declval<Fn>(), std::declval<V>().value())))
            -> decltype(invoke(std::declval<Fn>(), std::declval<V>().value())) {
        return invoke(std::forward<Fn>(fn), std::forward<V>(v).value());
    }
};

} // namespace tmdesc
