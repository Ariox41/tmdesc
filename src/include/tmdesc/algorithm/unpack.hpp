// Copyright Victor Smirnov 2021
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
//
// The documentation can be found at the library's page:
// https://github.com/Ariox41/tmdesc

#pragma once
#include "../functional/invoke.hpp"
#include "../get.hpp"
namespace tmdesc {
/** Invoke `fn` with `t` elements as arguments
@param t
    A tuple-like object with `get` and `tuple_size` implementation
@param fn
    A invokable object that accepts a set of tuple elements as a set of arguments
@return `fn` invoke result
*/
#ifdef TMDESC_DOXYGEN
constexpr auto unpack = [](auto&& t, auto&& fn) {
    static constexpr N = tuple_size_of(t);
    return invoke(fn, get<0>(t), /*...*/ get<N - 1>(t));
};
#else
struct unpack_t {
private:
    template <class Tuple, class Fn, std::size_t... I>
    constexpr decltype(auto) unpack_impl(Tuple&& tuple, Fn&& fn, std::index_sequence<I...>) const
        noexcept(noexcept(invoke(std::declval<Fn>(), get<I>(std::declval<Tuple>())...))) {
        return invoke(std::forward<Fn>(fn), get<I>(std::forward<Tuple>(tuple))...);
    }

public:
    template <class Tuple, class Fn>
    constexpr decltype(auto) operator()(Tuple&& tuple, Fn&& fn) const
        noexcept(noexcept(unpack_impl(std::declval<Tuple>(), std::declval<Fn>(), index_sequence_for_tuple<Tuple>{}))) {
        return unpack_impl(std::forward<Tuple>(tuple), std::forward<Fn>(fn), index_sequence_for_tuple<Tuple>{});
    }
};

constexpr unpack_t unpack{};
#endif

/** Invoke `fn` with `t` elements as arguments
@param t
    A tuple-like object with `get` and `tuple_size` implementation
@param fn
    A invokable object that accepts a set of tuple elements as a set of arguments
@return `fn` invoke result
*/
#ifdef TMDESC_DOXYGEN
constexpr auto unpack = [](auto&& t1, auto&& t2, auto&& fn) {
    static constexpr N1 = tuple_size_of(t1);
    static constexpr N2 = tuple_size_of(t2);
    return invoke(fn, get<0>(t1), /*...*/ get<N1 - 1>(t1), get<0>(t2), /*...*/ get<N2 - 1>(t2));
};
#else
struct unpack2_t {
private:
    template <class Tuple1, class Tuple2, class Fn, std::size_t... I1, std::size_t... I2>
    constexpr decltype(auto) unpack2_impl(Tuple1&& t1, Tuple2&& t2, Fn&& fn, std::index_sequence<I1...>,
                                          std::index_sequence<I2...>) const
        noexcept(noexcept(invoke(std::declval<Fn>(), get<I1>(std::declval<Tuple1>())...,
                                 get<I2>(std::declval<Tuple2>())...))) {
        return invoke(std::forward<Fn>(fn), get<I1>(std::forward<Tuple1>(t1))..., get<I2>(std::forward<Tuple2>(t2))...);
    }

public:
    template <class Tuple1, class Tuple2, class Fn>
    constexpr decltype(auto) operator()(Tuple1&& t1, Tuple2&& t2, Fn&& fn) const
        noexcept(noexcept(unpack2_impl(std::declval<Tuple1>(), std::declval<Tuple2>(), std::declval<Fn>(),
                                       index_sequence_for_tuple<Tuple1>{}, index_sequence_for_tuple<Tuple2>{}))) {
        return unpack2_impl(std::forward<Tuple1>(t1), std::forward<Tuple2>(t2), std::forward<Fn>(fn),
                            index_sequence_for_tuple<Tuple1>{}, index_sequence_for_tuple<Tuple2>{});
    }
};

constexpr unpack2_t unpack2{};
#endif
} // namespace tmdesc
