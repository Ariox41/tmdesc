// Copyright Victor Smirnov 2021-2022
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
//
// The documentation can be found at the library's page:
// https://github.com/Ariox41/tmdesc

// #pragma once
// #include "get_type_info.hpp"
// #include <boost/hana/range.hpp>
// #include <boost/hana/transform.hpp>
// #include <boost/hana/size.hpp>
// namespace tmdesc {

// /// Reference to a member of the struct
// template <class S, std::size_t I> struct member_ref {
//     explicit member_ref(S&& struct_ref)
//       : struct_ref_(static_cast<S&&>(struct_ref)) {}
//     static constexpr zstring_view name() noexcept {
//         return boost::hana::at_c<I>(get_type_members_info(boost::hana::type_c<S>).value()).name();
//     }
//     static constexpr auto type() noexcept {
//         return boost::hana::at_c<I>(get_type_members_info(boost::hana::type_c<S>).value()).type();
//     }
//     static constexpr decltype(auto) attributes() noexcept {
//         return boost::hana::at_c<I>(get_type_members_info(boost::hana::type_c<S>).value()).attributes();
//     }
//     constexpr decltype(auto) get() const noexcept {
//         return get_type_members_info(boost::hana::type_c<S>).value().accessor()(struct_ref_);
//     }

// private:
//     S&& struct_ref_;
// };

// namespace detail {
// template <class S> struct make_member_ref_t {
//     S&& s_;
//     template <class... I> constexpr auto operator()(I...) noexcept {
//         // TODO tuple, or some foldable object?
//         return boost::hana::make_basic_tuple(member_ref<S&&, I{}>(static_cast<S&&>(s_))...);
//     }
// };
// } // namespace detail

// template <
//     class S,
//     std::enable_if_t<!boost::hana::is_nothing(type_members_info_of(boost::hana::type_c<std::decay_t<S>>)), bool> = true>
// constexpr auto members_view(S&& s) {
//     return boost::hana::unpack(
//         boost::hana::make_range(boost::hana::size_c<0>,
//                                 boost::hana::length(get_type_members_info(boost::hana::type_c<S>).value())),
//         detail::make_member_ref_t<S&&>{static_cast<S&&>(s)});
// }
// } // namespace tmdesc
