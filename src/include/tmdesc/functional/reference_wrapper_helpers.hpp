// Copyright Victor Smirnov 2021
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
//
// The documentation can be found at the library's page:
// https://github.com/Ariox41/tmdesc

#pragma once
#include <type_traits>

namespace tmdesc {

template <class T> struct is_reference_wrapper : std::false_type {};
template <class U> struct is_reference_wrapper<std::reference_wrapper<U>> : std::true_type {};

template <class T> struct unwrap_reference_wrapper { using type = T; };

template <class T> struct unwrap_reference_wrapper<std::reference_wrapper<T>> { using type = T&; };

template <class T>
using unwrap_ref_decay_t = typename unwrap_reference_wrapper<std::decay_t<T>>::type;

} // namespace tmdesc
