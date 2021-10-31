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
    A tuple-like object with `get` and `tuple_size` operations

    @param fn
    A invocable object that accepts a set of tuple elements as a set of arguments

    @return `fn` invoke result
 */
#ifdef TMDESC_DOXYGEN
template <template <class...> class Consumer>
constexpr auto unpack = [](auto&& t, auto&& fn) {
    return invoke(fn, get<0>(t),
                  /*...*/
                  get<N - 1>(t));
};
#else
struct unpack_t {
private:
    template <class Tuple, class Fn, std::size_t... I>
    constexpr decltype(auto) unpack_impl(Tuple&& tuple, Fn&& fn, std::index_sequence<I...>) const
        noexcept(noexcept(::tmdesc::invoke(std::declval<Fn>(),
                                           ::tmdesc::get<I>(std::declval<Tuple>())...))) {
        return ::tmdesc::invoke(std::forward<Fn>(fn),
                                ::tmdesc::get<I>(std::forward<Tuple>(tuple))...);
    }

public:
    template <class Tuple, class Fn>
    constexpr decltype(auto) operator()(Tuple&& tuple, Fn&& fn) const
        noexcept(noexcept(unpack_impl(std::declval<Tuple>(), std::declval<Fn>(),
                                      ::tmdesc::index_sequence_for_tuple<Tuple>{}))) {
        return unpack_impl(std::forward<Tuple>(tuple), std::forward<Fn>(fn),
                           ::tmdesc::index_sequence_for_tuple<Tuple>{});
    }
};

constexpr unpack_t unpack{};
#endif
} // namespace tmdesc
