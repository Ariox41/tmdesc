// Copyright Victor Smirnov 2021
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
//
// The documentation can be found at the library's page:
// https://github.com/Ariox41/tmdesc
#pragma once
#include <type_traits>
namespace tmdesc {

/**
    Similar to std::optional, tmdesc::optional represents a value of some type or a non-existent value.

    Unlike std::optional, the optional-ness is known at compile time.

    @see some
    @see none
    @see is_some
    @see is_none
*/
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
template <> struct optional<> {
    constexpr optional()                = default;
    constexpr optional(optional&&)      = default;
    constexpr optional(const optional&) = default;
    constexpr optional& operator=(optional&&) = default;
    constexpr optional& operator=(const optional&) = default;

    // optional<>  does not contain a value
    const auto value() const = delete;
};

constexpr optional<> none{};

template <class T>
optional<std::decay_t<T>> some(T&& v) noexcept(std::is_nothrow_constructible<std::decay_t<T>, T&&>::value) {
    return optional<std::decay_t<T>>{static_cast<T&&>(v)};
}

template <class T, class U>
constexpr auto operator==(const optional<T>& lha, const optional<U>& rha) noexcept(noexcept(std::declval<const T&>() ==
                                                                                            std::declval<const U&>()))
    -> decltype(std::declval<const T&>() == std::declval<const U&>()) {
    return lha.value() == rha.value();
}
template <class T> constexpr bool operator==(const optional<T>&, const optional<>&) noexcept { return false; }
template <class T> constexpr bool operator==(const optional<>&, const optional<T>&) noexcept { return false; }
constexpr bool operator==(const optional<>&, const optional<>&) noexcept { return true; }

template <class T> using some_t = optional<T>;
using none_t                    = optional<>;

template <class T> struct is_optional : std::false_type {};
template <class T> struct is_optional<optional<T>> : std::true_type {};
template <> struct is_optional<optional<>> : std::true_type {};

template <class T> struct is_some : std::false_type {};

template <class T> struct is_some<some_t<T>> : std::true_type {};

template <class T, class Fn, class Ret = decltype(std::declval<Fn>()())>
constexpr const T& get_some_or_eval(const optional<T>& opt, Fn&&) noexcept {
    static_assert(std::is_convertible<Ret, T>::value, "");
    return opt.value();
}
template <class T, class Fn, class Ret = decltype(std::declval<Fn>()())>
constexpr T& get_some_or_eval(optional<T>& opt, Fn&&) noexcept {
    static_assert(std::is_convertible<Ret, T>::value, "");
    return opt.value();
}
template <class T, class Fn, class Ret = decltype(std::declval<Fn>()())>
constexpr T&& get_some_or_eval(optional<T>&& opt, Fn&&) noexcept {
    static_assert(std::is_convertible<Ret, T>::value, "");
    return std::move(opt).value();
}

template <class Opt, class Fn, std::enable_if_t<is_some<std::decay_t<Opt>>::value, bool> = true>
constexpr auto if_some(Opt&& opt, Fn&& fn) noexcept {
    return invoke(std::forward<Fn>(fn), std::forward<Opt>(opt).value());
}
template <class Fn> constexpr void if_some(none_t, Fn&&) noexcept {}

} // namespace tmdesc
