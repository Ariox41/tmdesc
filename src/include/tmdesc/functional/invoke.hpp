// Copyright Victor Smirnov 2023
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
//
// The documentation can be found at the library's page:
// https://github.com/Ariox41/tmdesc

#pragma once
#include "ref_obj_fwd.hpp"
#include <type_traits>
#include <utility>

namespace tmdesc {

using std::invoke_result;
using std::invoke_result_t;

using std::is_invocable;
using std::is_invocable_r;
using std::is_nothrow_invocable;
using std::is_nothrow_invocable_r;

using std::is_invocable_r_v;
using std::is_invocable_v;
using std::is_nothrow_invocable_r_v;
using std::is_nothrow_invocable_v;

namespace detail {

template <class C, class Pointed, class T1, class... Args>
constexpr decltype(auto) invoke_memptr(Pointed C::*f, T1&& t1, Args&&... args) {
    if constexpr (std::is_function_v<Pointed>) {
        if constexpr (std::is_base_of_v<C, std::decay_t<T1>>)
            return (std::forward<T1>(t1).*f)(std::forward<Args>(args)...);
        else if constexpr (tmdesc::is_ref_obj_v<std::decay_t<T1>>)
            return (t1.get().*f)(std::forward<Args>(args)...);
        else
            return ((*std::forward<T1>(t1)).*f)(std::forward<Args>(args)...);
    } else {
        static_assert(std::is_object_v<Pointed> && sizeof...(args) == 0);
        if constexpr (std::is_base_of_v<C, std::decay_t<T1>>)
            return std::forward<T1>(t1).*f;
        else if constexpr (tmdesc::is_ref_obj_v<std::decay_t<T1>>)
            return t1.get().*f;
        else
            return (*std::forward<T1>(t1)).*f;
    }
}

} // namespace detail

struct invoke_t {
    template <class F, class... Args>
    constexpr invoke_result_t<F, Args...> operator()(F&& f, Args&&... args) const
        noexcept(is_nothrow_invocable_v<F, Args...>) {
        if constexpr (std::is_member_pointer_v<std::decay_t<F>>)
            return tmdesc::detail::invoke_memptr(f, std::forward<Args>(args)...);
        else
            return std::forward<F>(f)(std::forward<Args>(args)...);
    }
};
constexpr invoke_t invoke{};

} // namespace tmdesc
