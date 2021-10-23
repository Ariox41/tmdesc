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
struct tuple_foreach_t {
private:
    struct impl_t {
        template <class Tuple, class Fn, std::size_t... I>
        constexpr void operator()(Tuple&& t, Fn&& fn, std::index_sequence<I...>) const noexcept( //
            ::tmdesc::meta::fast_and({
                noexcept(::tmdesc::invoke(std::declval<Fn&>(), get<I>(std::declval<Tuple>())))... //
            })) {
            (void)std::initializer_list<bool>{
                true, ((void)::tmdesc::invoke(static_cast<Fn&>(fn), get<I>(std::forward<Tuple>(t))),
                       void(), true)...};
        }
    };

public:
    template <class Tuple, class Fn>
    constexpr void operator()(Tuple&& t, Fn&& fn) const
        noexcept(noexcept(impl_t{}(std::declval<Tuple>(), std::declval<Fn>(),
                                   index_sequence_for_tuple<Tuple>{}))) {
        impl_t{}(std::forward<Tuple>(t), std::forward<Fn>(fn),
                 tmdesc::index_sequence_for_tuple<Tuple>{});
    }
};
constexpr tuple_foreach_t tuple_foreach{};

struct tuple_foreach2_t {
private:
    struct impl_t {
        template <class Tuple1, class Tuple2, class Fn, std::size_t... I>
        constexpr void operator()(Tuple1&& t1, Tuple2&& t2, Fn&& fn,
                                  std::index_sequence<I...>) const
            noexcept(::tmdesc::meta::fast_and(
                {noexcept(::tmdesc::invoke(std::declval<Fn&>(), get<I>(std::declval<Tuple1>()),
                                           get<I>(std::declval<Tuple2>())))...})) {
            (void)std::initializer_list<bool>{
                true,
                ((void)::tmdesc::invoke(static_cast<Fn&>(fn), get<I>(std::forward<Tuple1>(t1)),
                                        get<I>(std::forward<Tuple2>(t2))),
                 void(), true)...};
        }
    };

public:
    template <class Tuple1, class Tuple2, class Fn,
              std::enable_if_t<tuple_size_v<std::remove_reference_t<Tuple1>> ==
                                   tuple_size_v<std::remove_reference_t<Tuple2>>,
                               bool> = true>
    constexpr void operator()(Tuple1&& t1, Tuple2&& t2, Fn&& fn) const
        noexcept(noexcept(impl_t{}(std::declval<Tuple1>(), std::declval<Tuple2>(),
                                   std::declval<Fn>(), index_sequence_for_tuple<Tuple1>{}))) {
        impl_t{}(std::forward<Tuple1>(t1), std::forward<Tuple2>(t2), std::forward<Fn>(fn),
                 index_sequence_for_tuple<Tuple1>{});
    }
};
constexpr tuple_foreach2_t tuple_foreach2{};
} // namespace tmdesc
