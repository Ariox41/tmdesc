// // Copyright Victor Smirnov 2021
// // Distributed under the Boost Software License, Version 1.0.
// // (See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
// //
// // The documentation can be found at the library's page:
// // https://github.com/Ariox41/tmdesc

// #pragma once

// #include "../algorithm/for_each.hpp"
// #include "../functional/invoke.hpp"
// #include "tuple.hpp"

// namespace tmdesc {
// namespace detail {

// struct eq_visitor {
//     bool& ok;
//     template <class T, class U>
//     constexpr void operator()(const T& lha_, const U& rha_) const
//         noexcept(noexcept(!(std::declval<const T&>() == std::declval<const U&>()))) {
//             auto&& _lha = lha_;
//             auto&& _rha = rha_;
//         if (ok) {
//             if (!(_lha == _rha)) {
//                 ok = false;
//             }
//         }
//     }
// };
// struct less_visitor {
//     bool& less;
//     bool& eq;
//     template <class T, class U>
//     constexpr void operator()(const T& lha, const U& rha) const
//         noexcept(noexcept(std::declval<const T&>() <
//                           std::declval<const U&>()) && noexcept(std::declval<const U&>() <
//                                                                 std::declval<const T&>())) {
//         if (eq) {
//             if (lha < rha) {
//                 eq   = false;
//                 less = true;
//             } else if (rha < lha) {
//                 eq   = false;
//                 less = false;
//             }
//         }
//     }
// };
// } // namespace detail

// template <class... Ts, class... Us>
// constexpr std::enable_if_t<sizeof...(Ts) == sizeof...(Us), bool>
// operator==(const tuple<Ts...>& lha, const tuple<Us...>& rha) noexcept(
//     noexcept(for_each2(std::declval<const tuple<Ts...>&>(), std::declval<const tuple<Us...>&>(),
//                        std::declval<detail::eq_visitor>()))) {
//     bool eq = true;
//     for_each2(lha, rha, detail::eq_visitor{eq});
//     return eq;
// }
// template <class... Ts, class... Us>
// constexpr std::enable_if_t<sizeof...(Ts) == sizeof...(Us), bool>
// operator!=(const tuple<Ts...>& lha, const tuple<Us...>& rha) noexcept(noexcept(!(lha == rha))) {
//     return !(lha == rha);
// }

// template <class... Ts, class... Us>
// constexpr std::enable_if_t<sizeof...(Ts) == sizeof...(Us), bool>
// operator<(const tuple<Ts...>& lha, const tuple<Us...>& rha) noexcept(
//     noexcept(for_each2(lha, rha, std::declval<detail::less_visitor>()))) {
//     bool less = false;
//     bool eq   = true;
//     for_each2(lha, rha, detail::less_visitor{less, eq});
//     return less;
// }
// template <class... Ts, class... Us>
// constexpr std::enable_if_t<sizeof...(Ts) == sizeof...(Us), bool>
// operator>(const tuple<Ts...>& lha, const tuple<Us...>& rha) noexcept(noexcept(rha < lha)) {
//     return rha < lha;
// }
// template <class... Ts, class... Us>
// constexpr std::enable_if_t<sizeof...(Ts) == sizeof...(Us), bool>
// operator<=(const tuple<Ts...>& lha, const tuple<Us...>& rha) noexcept(noexcept(rha < lha)) {
//     return !(rha < lha);
// }
// template <class... Ts, class... Us>
// constexpr std::enable_if_t<sizeof...(Ts) == sizeof...(Us), bool>
// operator>=(const tuple<Ts...>& lha, const tuple<Us...>& rha) noexcept(noexcept(lha < rha)) {
//     return !(lha < rha);
// }

// } // namespace tmdesc
