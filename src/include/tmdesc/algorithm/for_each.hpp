// Copyright Victor Smirnov 2021
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
//
// The documentation can be found at the library's page:
// https://github.com/Ariox41/tmdesc

#pragma once
#include "../functional/invoke.hpp"
#include "../get.hpp"
#include "../meta/logical_operations.hpp"

namespace tmdesc {

/** Call `fn` on each element of `t` and ignore the result

    @param t
    A tuple-like object with `get` and `tuple_size` operations

    @param fn
    A unary invokable object overloaded for each element of the `t`
 */
#ifdef TMDESC_DOXYGEN
constexpr auto for_each = [](auto&& t, auto&& fn) -> void {
    invoke(fn, get<0>(t));
    /*...*/
    invoke(fn, get<N - 1>(t));
};
#else
struct for_each_t {
private:
    template <class Tuple, class Fn, std::size_t... I>
    static constexpr void for_each_impl(Tuple&& t, Fn&& fn, std::index_sequence<I...>) noexcept(                     //
        ::tmdesc::meta::fast_and_v<noexcept(::tmdesc::invoke(std::declval<Fn&>(), get<I>(std::declval<Tuple>())))... //
                                   >) {
        (void)std::initializer_list<bool>{
            true, ((void)::tmdesc::invoke(static_cast<Fn&>(fn), get<I>(std::forward<Tuple>(t))), void(), true)...};
    }

public:
    template <class Tuple, class Fn>
    constexpr void operator()(Tuple&& t, Fn&& fn) const
        noexcept(noexcept(for_each_impl(std::declval<Tuple>(), std::declval<Fn>(),
                                        index_sequence_for_tuple<Tuple>{}))) {
        for_each_impl(std::forward<Tuple>(t), std::forward<Fn>(fn), tmdesc::index_sequence_for_tuple<Tuple>{});
    }
};
constexpr for_each_t for_each{};
#endif

/** Call `fn` on each pair of elements of `t1` and `t2` with the same index and ignore the result

    @param t1
    A tuple-like object with `get` and `tuple_size` operations

    @param t2
    A tuple-like object with `get` and `tuple_size` operations
    @note the tuple_size of `t2` must be equal to the tuple_size of `t1`

    @param fn
    Binary callable object overloaded for each pair of elements of 't1' and `t2' with the same index
 */
#ifdef TMDESC_DOXYGEN
constexpr auto for_each2 = [](auto&& t1, auto&& t2, auto&& fn) -> void {
    invoke(fn, get<0>(t1), get<0>(t2));
    /*...*/
    invoke(fn, get<N - 1>(t1), get<N - 1>(t2));
};
#else
struct for_each2_t {
private:
    template <class Tuple1, class Tuple2, class Fn, std::size_t... I>
    static constexpr void for_each_impl(Tuple1&& t1, Tuple2&& t2, Fn&& fn, std::index_sequence<I...>) noexcept(
        ::tmdesc::meta::fast_and_v<noexcept(::tmdesc::invoke(std::declval<Fn&>(), get<I>(std::declval<Tuple1>()),
                                                             get<I>(std::declval<Tuple2>())))...>) {
        (void)std::initializer_list<bool>{
            true, ((void)::tmdesc::invoke(static_cast<Fn&>(fn), get<I>(std::forward<Tuple1>(t1)),
                                          get<I>(std::forward<Tuple2>(t2))),
                   void(), true)...};
    }

public:
    template <
        class Tuple1, class Tuple2, class Fn,
        std::enable_if_t<tuple_size_v<std::remove_reference_t<Tuple1>> == tuple_size_v<std::remove_reference_t<Tuple2>>,
                         bool> = true>
    constexpr void operator()(Tuple1&& t1, Tuple2&& t2, Fn&& fn) const
        noexcept(noexcept(for_each_impl(std::declval<Tuple1>(), std::declval<Tuple2>(), std::declval<Fn>(),
                                        index_sequence_for_tuple<Tuple1>{}))) {
        for_each_impl(std::forward<Tuple1>(t1), std::forward<Tuple2>(t2), std::forward<Fn>(fn),
                      index_sequence_for_tuple<Tuple1>{});
    }
};
constexpr for_each2_t for_each2{};
#endif
} // namespace tmdesc
