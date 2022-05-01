// Copyright Victor Smirnov 2021
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
//
// The documentation can be found at the library's page:
// https://github.com/Ariox41/tmdesc

#pragma once

#include <type_traits> //< for integral_constant
#include <utility>     //< for integer_sequence

namespace tmdesc {

/// `std::integral_constant` alternative with operators
///
/// @note no implicit conversion for math operations
template <class T, T V> struct integral_constant : std::integral_constant<T, V> {
    constexpr integral_constant<T, -V> operator-() const noexcept { return {}; }
};
template <std::size_t I> using size_constant = std::integral_constant<std::size_t, I>;
template <std::size_t I> constexpr size_constant<I> size_c{};

template <bool V> struct integral_constant<bool, V> : std::integral_constant<bool, V> {
    constexpr integral_constant<bool, !V> operator!() const noexcept { return {}; }
    constexpr auto operator-() const noexcept = delete;
};
template <bool B> using bool_constant = integral_constant<bool, B>;
using true_type                       = bool_constant<true>;
using false_type                      = bool_constant<false>;
constexpr true_type true_c{};
constexpr false_type false_c{};
template <bool B> constexpr bool_constant<B> bool_c{};

using std::index_sequence;
using std::integer_sequence;

template <std::size_t I> constexpr std::make_index_sequence<I> index_sequence_up_to(size_constant<I>) noexcept {
    return {};
}

template <class T, T v1, class U, U v2>
constexpr integral_constant<decltype(v1 << v2), (v1 << v2)> operator<<(integral_constant<T, v1>,
                                                                       integral_constant<U, v2>) noexcept {
    return {};
}
template <class T, T v1, class U, U v2>
constexpr integral_constant<decltype(v1 >> v2), (v1 >> v2)> operator>>(integral_constant<T, v1>,
                                                                       integral_constant<U, v2>) noexcept {
    return {};
}
template <class T, T v1, class U, U v2>
constexpr integral_constant<decltype(v1 | v2), (v1 | v2)> operator|(integral_constant<T, v1>,
                                                                    integral_constant<U, v2>) noexcept {
    return {};
}
template <class T, T v1, class U, U v2>
constexpr integral_constant<decltype(v1 & v2), (v1 & v2)> operator&(integral_constant<T, v1>,
                                                                    integral_constant<U, v2>) noexcept {
    return {};
}
template <class T, T v1, class U, U v2>
constexpr integral_constant<decltype(v1 ^ v2), (v1 ^ v2)> operator^(integral_constant<T, v1>,
                                                                    integral_constant<U, v2>) noexcept {
    return {};
}

template <class T, T v1, class U, U v2>
constexpr integral_constant<decltype(v1 || v2), (v1 || v2)> operator||(integral_constant<T, v1>,
                                                                       integral_constant<U, v2>) noexcept {
    return {};
}
template <class T, T v1, class U, U v2>
constexpr integral_constant<decltype(v1 && v2), (v1 && v2)> operator&&(integral_constant<T, v1>,
                                                                       integral_constant<U, v2>) noexcept {
    return {};
}
template <class T, T v1, class U, U v2>
constexpr integral_constant<decltype(v1 + v2), (v1 + v2)> operator+(integral_constant<T, v1>,
                                                                    integral_constant<U, v2>) noexcept {
    return {};
}
template <class T, T v1, class U, U v2>
constexpr integral_constant<decltype(v1 - v2), (v1 - v2)> operator-(integral_constant<T, v1>,
                                                                    integral_constant<U, v2>) noexcept {
    return {};
}
template <class T, T v1, class U, U v2>
constexpr integral_constant<decltype(v1 * v2), (v1 * v2)> operator*(integral_constant<T, v1>,
                                                                    integral_constant<U, v2>) noexcept {
    return {};
}
template <class T, T v1, class U, U v2>
constexpr integral_constant<decltype(v1 / v2), (v1 / v2)> operator/(integral_constant<T, v1>,
                                                                    integral_constant<U, v2>) noexcept {
    return {};
}
template <class T, T v1, class U, U v2>
constexpr integral_constant<decltype(v1 == v2), (v1 == v2)> operator==(integral_constant<T, v1>,
                                                                       integral_constant<U, v2>) noexcept {
    return {};
}
template <class T, T v1, class U, U v2>
constexpr integral_constant<decltype(v1 != v2), (v1 != v2)> operator!=(integral_constant<T, v1>,
                                                                       integral_constant<U, v2>) noexcept {
    return {};
}
template <class T, T v1, class U, U v2>
constexpr integral_constant<decltype(v1 < v2), (v1 < v2)> operator<(integral_constant<T, v1>,
                                                                    integral_constant<U, v2>) noexcept {
    return {};
}
template <class T, T v1, class U, U v2>
constexpr integral_constant<decltype(v1 > v2), (v1 > v2)> operator>(integral_constant<T, v1>,
                                                                    integral_constant<U, v2>) noexcept {
    return {};
}
template <class T, T v1, class U, U v2>
constexpr integral_constant<decltype(v1 <= v2), (v1 <= v2)> operator<=(integral_constant<T, v1>,
                                                                       integral_constant<U, v2>) noexcept {
    return {};
}
template <class T, T v1, class U, U v2>
constexpr integral_constant<decltype(v1 >= v2), (v1 >= v2)> operator>=(integral_constant<T, v1>,
                                                                       integral_constant<U, v2>) noexcept {
    return {};
}
} // namespace tmdesc