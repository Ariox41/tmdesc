// Copyright Victor Smirnov 2021
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)

#pragma once
#include <type_traits>
namespace tmdesc {
namespace meta {

template <class T> struct remove_cvref {
    using type = std::remove_cv_t<std::remove_reference_t<T>>;
};
template <class T> using remove_cvref_t = typename remove_cvref<T>::type;

template <template <class...> class Target, class T> struct apply_from;
template <template <class...> class Target, template <class...> class Tuple, class... Args>
struct apply_from<Target, Tuple<Args...>> {
    using type = Target<Args...>;
};

template <template <class...> class Target, class T>
using apply_from_t = typename apply_from<Target, T>::type;

} // namespace meta
} // namespace tmdesc
