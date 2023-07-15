// Copyright Victor Smirnov 2021
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
//
// The documentation can be found at the library's page:
// https://github.com/Ariox41/tmdesc

#pragma once
#include <tmdesc/functional/invoke.hpp>
#include <utility>

namespace tmdesc {
namespace details {
template <std::size_t n, typename Enable = void> struct foldl_impl;

template <> struct foldl_impl<1> {
    template <typename Fn, typename A1> static constexpr auto apply(Fn&&, A1&& a1) noexcept { //
        return std::forward<A1>(a1);
    }
};
template <> struct foldl_impl<2> {
    template <typename Fn, typename A1, typename A2>
    static constexpr auto apply(Fn&& fn, A1&& a1, A2&& a2)                               //
        noexcept(is_nothrow_invocable_v<Fn, A1, A2>) -> invoke_result_t<Fn, A1, A2> {    //
        return invoke(std::forward<Fn>(fn), std::forward<A1>(a1), std::forward<A2>(a2)); //
    }
};

template <> struct foldl_impl<3> {
    template <typename Fn, typename A1, typename A2, typename A3>
    static constexpr auto apply(Fn&& fn, A1&& a1, A2&& a2, A3&& a3)              //
        noexcept(noexcept(invoke(std::declval<Fn&>(),                            //
                                 invoke(std::declval<Fn&>(),                     //
                                        std::declval<A1>(), std::declval<A2>()), //
                                 std::declval<A3>())))                           //
        -> decltype(invoke(std::declval<Fn&>(),                                  //
                           invoke(std::declval<Fn&>(),                           //
                                  std::declval<A1>(), std::declval<A2>()),       //
                           std::declval<A3>())) {                                //
        return invoke(fn,                                                        //
                      invoke(fn,                                                 //
                             std::forward<A1>(a1), std::forward<A2>(a2)),        //
                      std::forward<A3>(a3));                                     //
    }
};
template <> struct foldl_impl<4> {
    template <typename Fn, typename A1, typename A2, typename A3, typename A4>
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
};
template <> struct foldl_impl<5> {
    template <typename Fn, typename A1, typename A2, typename A3, typename A4, typename A5>
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
};

template <std::size_t n> struct foldl_impl<n, std::enable_if_t<(n >= 6)>> {
    template <typename Fn, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename... As>
    static constexpr auto apply(Fn&& fn, A1&& a1, A2&& a2, A3&& a3, A4&& a4, A5&& a5, A6&& a6,
                                As&&... as)                                                 //
        noexcept(                                                                           //
            noexcept(foldl_impl<1 + sizeof...(As)>::apply(                                  //
                std::declval<Fn&>(),                                                        //
                invoke(std::declval<Fn&>(),                                                 //
                       invoke(std::declval<Fn&>(),                                          //
                              invoke(std::declval<Fn&>(),                                   //
                                     invoke(std::declval<Fn&>(),                            //
                                            invoke(std::declval<Fn&>(),                     //
                                                   std::declval<A1>(),                      //
                                                   std::declval<A2>()),                     //
                                            std::declval<A3>()),                            //
                                     std::declval<A4>()),                                   //
                              std::declval<A5>()),                                          //
                       std::declval<A6>()),                                                 //
                std::declval<As>()...))                                                     //
            )                                                                               //
        -> decltype(foldl_impl<1 + sizeof...(As)>::apply(                                   //
            std::declval<Fn&>(),                                                            //
            invoke(std::declval<Fn&>(),                                                     //
                   invoke(std::declval<Fn&>(),                                              //
                          invoke(std::declval<Fn&>(),                                       //
                                 invoke(std::declval<Fn&>(),                                //
                                        invoke(std::declval<Fn&>(),                         //
                                               std::declval<A1>(),                          //
                                               std::declval<A2>()),                         //
                                        std::declval<A3>()),                                //
                                 std::declval<A4>()),                                       //
                          std::declval<A5>()),                                              //
                   std::declval<A6>()),                                                     //
            std::declval<As>()...)) {                                                       //
        return foldl_impl<1 + sizeof...(As)>::apply(                                        //
            fn,                                                                             //
            invoke(fn,                                                                      //
                   invoke(fn,                                                               //
                          invoke(fn,                                                        //
                                 invoke(fn,                                                 //
                                        invoke(fn,                                          //
                                               std::forward<A1>(a1), std::forward<A2>(a2)), //
                                        std::forward<A3>(a3)),                              //
                                 std::forward<A4>(a4)),                                     //
                          std::forward<A5>(a5)),                                            //
                   std::forward<A6>(a6)),                                                   //
            std::forward<As>(as)...);
    }
};
} // namespace details
struct variadic_fold_left_t {
    template <typename Fn, typename A1, typename... As>
    constexpr auto operator()(Fn&& fn, A1&& a1, As&&... as) const
        noexcept(noexcept(tmdesc::details::foldl_impl<1 + sizeof...(As)>::apply(std::declval<Fn>(), std::declval<A1>(),
                                                                                std::declval<As>()...)))
            -> decltype(tmdesc::details::foldl_impl<1 + sizeof...(As)>::apply(std::declval<Fn>(), std::declval<A1>(),
                                                                              std::declval<As>()...)) {
        return tmdesc::details::foldl_impl<1 + sizeof...(As)>::apply(std::forward<Fn>(fn), std::forward<A1>(a1),
                                                                     std::forward<As>(as)...);
    }
};

/// fold(f, x1) => f(x1)
/// fold(f, x1, x2) => f(x1, x2)
/// fold(f, x1, x2, x3) => f(f(x1, x2), x3)
/// fold(f, x1, x2, ... xn) => f(f(...f(x1, x2)...), xn)
///
/// @note `fn(args...)` is a `invoke(fn, args...)`
inline constexpr variadic_fold_left_t variadic_fold_left{};

} // namespace tmdesc
