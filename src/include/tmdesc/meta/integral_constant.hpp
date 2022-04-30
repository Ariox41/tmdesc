// Copyright Victor Smirnov 2021
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
//
// The documentation can be found at the library's page:
// https://github.com/Ariox41/tmdesc

#pragma once
#include <type_traits>
#include <utility>

namespace tmdesc {

using std::false_type;
using std::integral_constant;
using std::true_type;

using std::index_sequence;
using std::integer_sequence;

template <bool B> using bool_constant = std::integral_constant<bool, B>;
template <bool B> constexpr bool_constant<B> bool_c{};

template <std::size_t I> using size_constant = std::integral_constant<std::size_t, I>;
template <std::size_t I> constexpr size_constant<I> size_c{};

template <std::size_t I> constexpr std::make_index_sequence<I> index_sequence_up_to(size_constant<I>) noexcept { return {}; }

} // namespace tmdesc