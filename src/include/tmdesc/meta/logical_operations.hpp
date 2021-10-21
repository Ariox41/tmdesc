// Copyright Victor Smirnov 2021
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)

#pragma once
#include <initializer_list>
#include <type_traits>

namespace tmdesc {
namespace meta {

template <class BoolT> using not_t = std::integral_constant<bool, !BoolT::value>;

template <bool b, class T = void> using enable_if_t  = typename std::enable_if<b, T>::type;
template <bool b, class T = void> using disable_if_t = typename std::enable_if<!b, T>::type;

inline constexpr bool fast_and(std::initializer_list<bool> list) noexcept {
    bool ret = true;
    for (bool v : list) {
        ret = ret && v;
    }
    return ret;
}
inline constexpr bool fast_or(std::initializer_list<bool> list) noexcept {
    bool ret = false;
    for (bool v : list) {
        ret = ret || v;
    }
    return ret;
}

template <class... Bools> constexpr bool conjunction_v     = fast_and({Bools::value...});
template <bool... bools> constexpr bool bool_conjunction_v = fast_and({bools...});

template <class... Bools>
struct conjunction : std::integral_constant<bool, conjunction_v<Bools...>> {};

template <bool... bools>
struct bool_conjunction : std::integral_constant<bool, bool_conjunction_v<bools...>> {};

template <class... Bools> constexpr bool disjunction_v     = fast_or({Bools::value...});
template <bool... bools> constexpr bool bool_disjunction_v = fast_or({bools...});

template <class... Bools>
struct disjunction : std::integral_constant<bool, disjunction_v<Bools...>> {};

template <bool... bools>
struct bool_disjunction : std::integral_constant<bool, bool_disjunction_v<bools...>> {};

} // namespace meta
} // namespace tmdesc