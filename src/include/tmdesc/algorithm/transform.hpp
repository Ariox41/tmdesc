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
template <template <class...> class Consumer> struct transform_t {
private:
    template <class Tuple, class Fn, std::size_t... I>
    static constexpr decltype(auto)
    transform_impl(Tuple&& t, Fn&& fn, std::index_sequence<I...>) noexcept(
        noexcept(Consumer<invoke_result_t<Fn&, tuple_get_result_t<I, Tuple&&>>...>{
            invoke(std::declval<Fn&>(), std::declval<tuple_get_result_t<I, Tuple&&>>())...})) {
        return Consumer<invoke_result_t<Fn, tuple_get_result_t<I, Tuple&&>>...>{
            invoke(static_cast<Fn&>(fn), get<I>(std::forward<Tuple>(t)))...};
    }

public:
    template <class Tuple, class Fn>
    constexpr decltype(auto) operator()(Tuple&& t, Fn&& fn) const
        noexcept(noexcept(transform_impl(std::declval<Tuple>(), std::declval<Fn>(),
                                         index_sequence_for_tuple<Tuple>{}))) {
        return transform_impl(std::forward<Tuple>(t), std::forward<Fn>(fn),
                              index_sequence_for_tuple<Tuple>{});
    }
};

/// Apply function to each element of tuple-like object and return Consumer<Result...>(results...)
template <template <class...> class Consumer> constexpr transform_t<Consumer> transform_to{};

template <template <class...> class Consumer> struct transform2_t {
private:
    template <class Tuple1, class Tuple2, class Fn, std::size_t... I>
    static decltype(auto)
    transform_impl(Tuple1&& t1, Tuple2&& t2, Fn&& fn, std::index_sequence<I...>) noexcept(
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
        noexcept(noexcept(transform_impl(std::declval<Tuple1>(), std::declval<Tuple2>(),
                                         std::declval<Fn>(), index_sequence_for_tuple<Tuple1>{}))) {
        return transform_impl(std::forward<Tuple1>(t1), std::forward<Tuple2>(t2),
                              std::forward<Fn>(fn), index_sequence_for_tuple<Tuple1>{});
    }
};

/// Apply function to each pair of same index element of tuple-like object,
/// and return Consumer<Result...>(results...)
template <template <class...> class Consumer> constexpr transform2_t<Consumer> transform2_to{};

namespace detail {
template <class T> struct to_struct_helper { template <class... Args> using apply = T; };
} // namespace detail

/// Apply function to each element of tuple-like object and return T{results...}
template <class T>
constexpr transform_t<detail::to_struct_helper<T>::template apply> transform_to_type;

/// Apply function to each pair of same index element of tuple-like object,
/// and return T{results...}
template <class T>
constexpr transform2_t<detail::to_struct_helper<T>::template apply> transform2_to_type;

} // namespace tmdesc
