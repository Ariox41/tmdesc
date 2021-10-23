// Copyright Victor Smirnov 2021
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
//
// The documentation can be found at the library's page:
// https://github.com/Ariox41/tmdesc

namespace tmdesc {
namespace meta {

/// empty optonal template value (nothing, undefined)
struct none {
private:
    struct use_generic_lambda_in_if_none {};

public:
    template <class Fn> constexpr void if_some(Fn&&) const noexcept {}
    template <class Fn>
    constexpr void if_none(Fn&& fn) const
        noexcept(noexcept(static_cast<Fn&&>(fn)(use_generic_lambda_in_if_none{}))) {
        static_cast<Fn&&>(fn)(use_generic_lambda_in_if_none{});
    }
    static constexpr bool is_some() noexcept { return false; }
};

/// Some(Flag) (non empty optonal value)
template <class T> struct some {
private:
    T value;

public:
    constexpr some(T v) noexcept(std::is_nothrow_constructible<T, T&&>{})
      : value(static_cast<T&&>(v)) {}

    template <class Fn>
    constexpr void if_some(Fn&& fn) const
        noexcept(noexcept(static_cast<Fn&&>(fn)(std::declval<const T&>()))) {
        static_cast<Fn&&>(fn)(value);
    }
    template <class Fn> constexpr void if_none(Fn&&) const noexcept {}

    static constexpr bool is_some() noexcept { return true; }
};
} // namespace meta
} // namespace tmdesc