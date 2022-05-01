// Copyright Victor Smirnov 2021
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
//
// The documentation can be found at the library's page:
// https://github.com/Ariox41/tmdesc

#pragma once
#include "invoke.hpp"
#include "variadic_fold_left.hpp"
namespace tmdesc {

template <class T> struct construct_t {
    template <class... Args>
    constexpr auto operator()(Args&&... args) const noexcept(std::is_nothrow_constructible<T, Args>)
        -> decltype(T{std::declval<Args>()...}) {
        return T{std::forward<Args>(args)...};
    }
};

/// construct<T>(args...) => T{args...}
template <class T> constexpr construct_t<T> construct{};
} // namespace tmdesc
