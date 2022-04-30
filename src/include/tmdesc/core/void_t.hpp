// Copyright Victor Smirnov 2021
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
//
// The documentation can be found at the library's page:
// https://github.com/Ariox41/tmdesc

#pragma once
namespace tmdesc {

/// see std::void_t
template<class... Ts> struct make_void { typedef void type;};
template<class... Ts> using void_t = typename make_void<Ts...>::type;

} // namespace tmdesc