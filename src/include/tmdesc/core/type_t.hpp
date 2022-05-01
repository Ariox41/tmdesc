// Copyright Victor Smirnov 2021
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
//
// The documentation can be found at the library's page:
// https://github.com/Ariox41/tmdesc

#pragma once

namespace tmdesc {
/// type with value-semantic
template <class T> struct type_t { using type = T; };

template <class T> constexpr type_t<T> type_c{};

} // namespace tmdesc
