// Copyright Victor Smirnov 2021
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)

#pragma once

#include "primitives.hpp"
#include <type_traits>
#include <utility>
namespace tmdesc {
namespace meta {
template <class BoolT> using not_t = std::integral_constant<bool, !BoolT::value>;

template <bool b, class T = void> using enable_if_t  = typename std::enable_if<b, T>::type;
template <bool b, class T = void> using disable_if_t = typename std::enable_if<!b, T>::type;

inline constexpr bool fast_and(std::initializer_list<bool> list) noexcept {
    bool ret = true;
    for (bool v : list) {
        ret = ret && v;
    }
    return ret;
}

template <class... Bools> constexpr bool conjunction_v     = fast_and({Bools::value...});
template <bool... bools> constexpr bool bool_conjunction_v = fast_and({bools...});

template <class... Bools> struct conjunction : std::integral_constant<bool, conjunction_v<Bools...>> {};
template <bool... bools> struct bool_conjunction : std::integral_constant<bool, bool_conjunction_v<bools...>> {};

template <class T> struct remove_cvref { using type = std::remove_cv_t<std::remove_reference_t<T>>; };
template <class T> using remove_cvref_t = typename remove_cvref<T>::type;

template <template <class...> class Target, class T> struct apply_from;
template <template <class...> class Target, template <class...> class Tuple, class... Args>
struct apply_from<Target, Tuple<Args...>> {
    using type = Target<Args...>;
};

template <template <class...> class Target, class T> using apply_from_t = typename apply_from<Target, T>::type;

/// empty optonal template value
struct none {
private:
    struct use_generic_lambda_in_if_none {};

public:
    template <class Fn> constexpr void if_some(Fn&&) const noexcept {}
    template <class Fn>
    constexpr void if_none(Fn&& fn) const noexcept(noexcept(std::forward<Fn>(fn)(use_generic_lambda_in_if_none{}))) {
        std::forward<Fn>(fn)(use_generic_lambda_in_if_none{});
    }
    static constexpr bool is_some() noexcept { return false; }
};

/// Some(Flag) (non empty optonal value)
template <class T> struct some {
private:
    T value;

public:
    constexpr some(T v) noexcept(std::is_nothrow_constructible<T, T&&>{})
      : value(std::move(v)) {}

    template <class Fn>
    constexpr void if_some(Fn&& fn) const noexcept(noexcept(std::forward<Fn>(fn)(std::declval<const T&>()))) {
        std::forward<Fn>(fn)(value);
    }
    template <class Fn> constexpr void if_none(Fn&&) const noexcept {}

    static constexpr bool is_some() noexcept { return true; }
};

} // namespace meta
} // namespace tmdesc
