// Copyright Victor Smirnov 2021
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
//
// The documentation can be found at the library's page:
// https://github.com/Ariox41/tmdesc

#pragma once
#include "invoke.hpp"
#include <utility>

namespace tmdesc {
struct variadic_fold_left_t {
    struct helper {
        template <class Fn, class A1>
        static constexpr auto apply(Fn&& fn, A1&& a1) noexcept(noexcept(invoke(std::declval<Fn>(), std::declval<A1>())))
            -> decltype(invoke(std::declval<Fn>(), std::declval<A1>())) {
            return invoke(std::forward<Fn>(fn), std::forward<A1>(a1));
        }
        template <class Fn, class A1, class A2>
        static constexpr auto apply(Fn&& fn, A1&& a1, A2&& a2) //
            noexcept(noexcept(invoke(std::declval<Fn>(), std::declval<A1>(), std::declval<A2>())))
                -> decltype(invoke(std::declval<Fn>(), std::declval<A1>(), std::declval<A2>())) {
            return invoke(std::forward<Fn>(fn), std::forward<A1>(a1), std::forward<A2>(a2));
        }
        template <class Fn, class A1, class A2, class A3>
        static constexpr auto apply(Fn&& fn, A1&& a1, A2&& a2, A3&& a3)              //
            noexcept(noexcept(invoke(invoke(std::declval<Fn>(),                      //
                                            std::declval<A1>(), std::declval<A2>()), //
                                     std::declval<A3>())))                           //
            -> decltype(invoke(invoke(std::declval<Fn>(),                            //
                                      std::declval<A1>(), std::declval<A2>()),       //
                               std::declval<A3>())) {                                //
            return invoke(invoke(std::forward<Fn>(fn),                               //
                                 std::forward<A1>(a1), std::forward<A2>(a2)),        //
                          std::forward<A3>(a3));                                     //
        }
        template <class Fn, class A1, class A2, class A3, class A4>
        static constexpr auto apply(Fn&& fn, A1&& a1, A2&& a2, A3&& a3, A4&& a4) noexcept( //
            noexcept(invoke(std::declval<Fn&>(),                                           //
                            invoke(std::declval<Fn&>(),                                    //
                                   invoke(std::declval<Fn&>(),                             //
                                          std::declval<A1>(), std::declval<A2>()),         //
                                   std::declval<A3>()),                                    //
                            std::declval<A4>())))                                          //
            -> decltype(invoke(std::declval<Fn&>(),                                        //
                               invoke(std::declval<Fn&>(),                                 //
                                      invoke(std::declval<Fn&>(),                          //
                                             std::declval<A1>(), std::declval<A2>()),      //
                                      std::declval<A3>()),                                 //
                               std::declval<A4>())) {                                      //
            return invoke(fn,                                                              //
                          invoke(fn,                                                       //
                                 invoke(fn,                                                //
                                        std::forward<A1>(a1), std::forward<A2>(a2)),       //
                                 std::forward<A3>(a3)),                                    //
                          std::forward<A4>(a4));                                           //
        }
        template <class Fn, class A1, class A2, class A3, class A4, class A5>
        static constexpr auto apply(Fn&& fn, A1&& a1, A2&& a2, A3&& a3, A4&& a4, A5&& a5) noexcept( //
            noexcept(invoke(std::declval<Fn&>(),                                                    //
                            invoke(std::declval<Fn&>(),                                             //
                                   invoke(std::declval<Fn&>(),                                      //
                                          invoke(std::declval<Fn&>(),                               //
                                                 std::declval<A1>(), std::declval<A2>()),           //
                                          std::declval<A3>()),                                      //
                                   std::declval<A4>()),                                             //
                            std::declval<A5>())))                                                   //
            -> decltype(invoke(std::declval<Fn&>(),                                                 //
                               invoke(std::declval<Fn&>(),                                          //
                                      invoke(std::declval<Fn&>(),                                   //
                                             invoke(std::declval<Fn&>(),                            //
                                                    std::declval<A1>(), std::declval<A2>()),        //
                                             std::declval<A3>()),                                   //
                                      std::declval<A4>()),                                          //
                               std::declval<A5>())) {                                               //
            return invoke(fn,                                                                       //
                          invoke(fn,                                                                //
                                 invoke(fn,                                                         //
                                        invoke(fn,                                                  //
                                               std::forward<A1>(a1), std::forward<A2>(a2)),         //
                                        std::forward<A3>(a3)),                                      //
                                 std::forward<A4>(a4)),                                             //
                          std::forward<A5>(a5));                                                    //
        }

        template <class Fn, class A1, class A2, class A3, class A4, class A5, class A6, class A7, class... As>
        static constexpr auto apply(Fn&& fn, A1&& a1, A2&& a2, A3&& a3, A4&& a4, A5&& a5, A6&& a6, As&&... as) noexcept(
            noexcept(helper::apply(std::declval<Fn&>(),
                                   helper::apply(std::declval<Fn&>(), //
                                                 std::declval<A1>(), std::declval<A2>(), std::declval<A3>(),
                                                 std::declval<A4>(), std::declval<A5>()),
                                   std::declval<A6>(), std::declval<As>()...)))
            -> decltype(helper::apply(std::declval<Fn&>(),
                                      helper::apply(std::declval<Fn&>(), //
                                                    std::declval<A1>(), std::declval<A2>(), std::declval<A3>(),
                                                    std::declval<A4>(), std::declval<A5>()),
                                      std::declval<A6>(), std::declval<As>()...)) {
            return helper::apply(fn,
                                 helper::apply(fn, //
                                               std::forward<A1>(a1), std::forward<A2>(a2), std::forward<A3>(a3),
                                               std::forward<A4>(a4), std::forward<A5>(a5)),
                                 std::forward<A6>(a6), std::forward<As>(as)...);
        }
    };

    template <class Fn, class A1, class... As>
    constexpr auto operator()(Fn&& fn, A1&& a1, As&&... as) const
        noexcept(noexcept(helper::apply(std::forward<Fn>(fn), std::forward<A1>(a1))))
            -> decltype(helper::apply(std::forward<Fn>(fn), std::forward<A1>(a1))) {
        return helper::apply(std::forward<Fn>(fn), std::forward<A1>(a1));
    }
};

/// fold(f, x1) => f(x1)
/// fold(f, x1, x2) => f(x1, x2)
/// fold(f, x1, x2, x3) => f(f(x1, x2), x3)
/// fold(f, x1, x2, ... xn) => f(f(...f(x1, x2)...), xn)
///
/// @note `fn(args...)` is a `invoke(fn, args...)`
constexpr variadic_fold_left_t variadic_fold_left;

} // namespace tmdesc
