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
template <template <class...> class Consumer> struct tuple_transform_t {
private:
    template <class Tuple, class Fn, std::size_t... I>
    static constexpr decltype(auto)
    transform(Tuple&& t, Fn&& fn, std::index_sequence<I...>) noexcept(
        noexcept(Consumer<invoke_result_t<Fn&, tuple_get_result_t<I, Tuple&&>>...>{
            invoke(std::declval<Fn&>(), std::declval<tuple_get_result_t<I, Tuple&&>>())...})) {
        return Consumer<invoke_result_t<Fn, tuple_get_result_t<I, Tuple&&>>...>{
            invoke(static_cast<Fn&>(fn), get<I>(std::forward<Tuple>(t)))...};
    }

public:
    template <class Tuple, class Fn>
    constexpr decltype(auto) operator()(Tuple&& t, Fn&& fn) const
        noexcept(noexcept(transform(std::declval<Tuple>(), std::declval<Fn>(),
                                    index_sequence_for_tuple<Tuple>{}))) {
        return transform(std::forward<Tuple>(t), std::forward<Fn>(fn),
                         index_sequence_for_tuple<Tuple>{});
    }
};

template <template <class...> class Consumer>
constexpr tuple_transform_t<Consumer> tuple_transform_to{};
constexpr tuple_transform_t<tuple> tuple_transform{};

template <template <class...> class Consumer> struct tuple_transform2_t {
private:
    template <class Tuple1, class Tuple2, class Fn, std::size_t... I>
    static decltype(auto)
    transform(Tuple1&& t1, Tuple2&& t2, Fn&& fn, std::index_sequence<I...>) noexcept(
        Consumer<invoke_result_t<Fn&, tuple_get_result_t<I, Tuple1&&>,
                                 tuple_get_result_t<I, Tuple2&&>>...>{
            invoke(std::declval<Fn&>(), std::declval<tuple_get_result_t<I, Tuple1&&>>(),
                   std::declval<tuple_get_result_t<I, Tuple2&&>>())...}) {
        return Consumer<invoke_result_t<Fn&, tuple_get_result_t<I, Tuple1&&>,
                                        tuple_get_result_t<I, Tuple2&&>>...>{
            invoke(static_cast<Fn&>(fn), get<I>(std::forward<Tuple1>(t1)),
                   get<I>(std::forward<Tuple2>(t2)))...};
    }

public:
    template <class Tuple1, class Tuple2, class Fn,
              std::enable_if_t<tuple_size_v<std::remove_reference_t<Tuple1>> ==
                                   tuple_size_v<std::remove_reference_t<Tuple2>>,
                               bool> = true>
    constexpr decltype(auto) operator()(Tuple1&& t1, Tuple2&& t2, Fn&& fn) const
        noexcept(noexcept(tuple_unpack(transform(std::declval<Tuple1>(), std::declval<Tuple2>(),
                                                 std::declval<Fn>(),
                                                 index_sequence_for_tuple<Tuple1>{})))) {
        return transform(std::forward<Tuple1>(t1), std::forward<Tuple2>(t2), std::forward<Fn>(fn),
                         index_sequence_for_tuple<Tuple1>{});
    }
};

template <template <class...> class Consumer>
constexpr tuple_transform2_t<Consumer> tuple_transform2_to{};
constexpr tuple_transform2_t<tuple> tuple_transform2{};

} // namespace tmdesc
