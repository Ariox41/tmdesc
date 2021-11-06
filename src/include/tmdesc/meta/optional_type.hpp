// Copyright Victor Smirnov 2021
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
//
// The documentation can be found at the library's page:
// https://github.com/Ariox41/tmdesc
#pragma once
#include <type_traits>
namespace tmdesc {
namespace meta {
/**
    The `none` type indicating a non-existent type or non-existent value

    The idea is similar to `std::optional`,
    but unlike it `none` or `some` is defined at compile time.

    @see some
*/
struct none {
private:
    struct use_generic_lambda_in_if_none {};

public:
    /**
        Ignore argument because it is a `none`

        The purpose of this method is similar to if constexpr(is_some()){ fn(value()); }

        @param fn
        A functional object that can be called with one argument.
        `fn` will not be called here, but it can be called in @ref some::if_some
    */
    template <class Fn> constexpr void if_some(Fn&& fn) const noexcept { (void)fn; }

    /**
        Call `fn` with dummy argument.

        The purpose of this method is similar to if constexpr(is_some()){} else{ fn(dummy_t{}); }

        @param fn
        A functional object called with a dummy argument that should be ignored.
        A dummy argument allows the use of generic lambda which will not be used if it is @ref some
    */
    template <class Fn>
    constexpr void if_none(Fn&& fn) const
        noexcept(noexcept(static_cast<Fn&&>(fn)(use_generic_lambda_in_if_none{}))) {
        static_cast<Fn&&>(fn)(use_generic_lambda_in_if_none{});
    }

    /// @return false because it is a `none`
    /// @see some::is_some
    static constexpr bool is_some() noexcept { return false; }
};

/**
    The `some` type indicating an existent type and value

    The idea is similar to `std::optional`,
    but unlike it `none` or `some` is defined at compile time.

    @see none
*/
template <class T> struct some {
private:
    T value_;

public:
    /// @return a stored value
    /// @note This method can be used in recursive_and with `if constexpr(if_some())`,
    /// but for compatibility with c++14, you need to use `if_some` and `if_none`.
    const T& value() const noexcept { return value_; }

    constexpr some(T v) noexcept(std::is_nothrow_constructible<T, T&&>{})
      : value_(static_cast<T&&>(v)) {}

    /**
        Call `fn` with value as argument.

        The purpose of this method is similar to if constexpr(is_some()){ fn(value()); }

        @param fn
        A functional object called with this->value() as an argument.
    */
    template <class Fn>
    constexpr void if_some(Fn&& fn) const
        noexcept(noexcept(static_cast<Fn&&>(fn)(std::declval<const T&>()))) {
        static_cast<Fn&&>(fn)(value());
    }
    /**
        Ignore argument because it is a `some`

        The purpose of this method is similar to if constexpr(is_some()){} else{ fn(dummy_t{}); }

        @param fn
        A functional object that can be called with a dummy argument that should be ignored.
        `fn` will not be called here, but it can be called in @ref none::if_none
    */
    template <class Fn> constexpr void if_none(Fn&& fn) const noexcept { (void)fn; }

    /// @return true because it is a `some`
    /// @see none::is_some
    static constexpr bool is_some() noexcept { return true; }
};
} // namespace meta
} // namespace tmdesc
