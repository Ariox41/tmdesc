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
/** Call `fn` on each element of `t` and return `Consumer` with result
    @tparam Consumer
    A tuple-like container for result values.
    `invoke_result` of `fn` will be set to the template parameters of the `Consumer`

    @param t
    A tuple-like object with `get` and `tuple_size` operations

    @param fn
    A unary invocable object overloaded for each element of the `t`

    @return `Consumer` of `fn` invoke result
 */
#ifdef TMDESC_DOXYGEN
template <template <class...> class Consumer>
constexpr auto transform_to =
    [](auto&& t, auto&& fn) -> Consumer<decltype(invoke(fn, get<0>(t))),
                                        /*...*/
                                        decltype(invoke(fn, get<N - 1>(t)))> {
    return {
        invoke(fn, get<0>(t));
        /*...*/
        invoke(fn, get<N - 1>(t))
    };
};
#else
template <template <class...> class Consumer> struct transform_t {
private:
    template <class Tuple, class Fn, std::size_t... I>
    static constexpr Consumer<invoke_result_t<Fn, tuple_get_result_t<I, Tuple&&>>...>
    transform_impl(Tuple&& t, Fn&& fn, std::index_sequence<I...>) noexcept(
        noexcept(Consumer<invoke_result_t<Fn&, tuple_get_result_t<I, Tuple&&>>...>{
            invoke(std::declval<Fn&>(), std::declval<tuple_get_result_t<I, Tuple&&>>())...})) {
        return {
            invoke(static_cast<Fn&>(fn), get<I>(std::forward<Tuple>(t)))...};
    }

public:
    template <class Tuple, class Fn>
    constexpr auto operator()(Tuple&& t, Fn&& fn) const
        noexcept(noexcept(transform_impl(std::declval<Tuple>(), std::declval<Fn>(),
                                         index_sequence_for_tuple<Tuple>{}))) {
        return transform_impl(std::forward<Tuple>(t), std::forward<Fn>(fn),
                              index_sequence_for_tuple<Tuple>{});
    }
};

/// Apply function to each element of tuple-like object and return Consumer<Result...>(results...)
template <template <class...> class Consumer> constexpr transform_t<Consumer> transform_to{};
#endif

/** Call `fn` on each pair of elements of `t1` and `t2` with the same index and
    return `Consumer` with result
    @tparam Consumer
    A tuple-like container for result values.
    `invoke_result` of `fn` will be set to the template parameters of the `Consumer`

    @param t1
    A tuple-like object with `get` and `tuple_size` operations

    @param t2
    A tuple-like object with `get` and `tuple_size` operations
    @note the tuple_size of `t2` must be equal to the tuple_size of `t1`

    @param fn
    Binary callable object overloaded for each pair of elements of 't1' and `t2' with the same index

    @return `Consumer` of `fn` invoke result
 */
#ifdef TMDESC_DOXYGEN
template <template <class...> class Consumer>
constexpr auto transform2_to =
    [](auto&& t1, auto&& t2,
       auto&& fn) -> Consumer<decltype(invoke(fn, get<0>(t1), get<0>(t2))),
                              /*...*/
                              decltype(invoke(fn, get<N - 1>(t1), get<N - 1>(t2)))> {
    return {
        invoke(fn, get<0>(t1), get<0>(t2));
        /*...*/
        invoke(fn, get<N - 1>(t1), get<N - 1>(t2))
    };
};
#else
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
#endif

namespace detail {
template <class T> struct type_as_template { template <class... Args> using apply = T; };
} // namespace detail

/// Similar to @ref transform_to, but a fully defined type `T` is used as `Consumer`
#ifdef TMDESC_DOXYGEN
template <class T>
constexpr auto transform_to_type = [](auto&& t, auto&& fn) -> T {
    return transform_to<detail::type_as_template<T>::template apply>(t, fn);
};
#else
template <class T>
constexpr transform_t<detail::type_as_template<T>::template apply> transform_to_type;
#endif

/// Similar to @ref transform2_to, but a fully defined type `T` is used as `Consumer`
#ifdef TMDESC_DOXYGEN
template <class T>
constexpr auto transform2_to_type = [](auto&& t, auto&& fn) -> T {
    return transform2_to<detail::type_as_template<T>::template apply>(t, fn);
};
#else
template <class T>
constexpr transform2_t<detail::type_as_template<T>::template apply> transform2_to_type;
#endif

} // namespace tmdesc
