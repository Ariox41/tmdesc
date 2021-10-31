// Copyright Victor Smirnov 2021
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
//
// The documentation can be found at the library's page:
// https://github.com/Ariox41/tmdesc

#pragma once
#include "../meta/void_t.hpp"
#include "reference_wrapper_helpers.hpp"
#include <type_traits>
#include <utility>

namespace tmdesc {
namespace detail {

struct memptr_invoker_t {
protected:
    struct ref_to_ref {
        template <class Ref> constexpr Ref&& operator()(Ref&& r) const noexcept {
            return std::forward<Ref>(r);
        }
    };
    struct ref_wrapper_to_ref {
        template <class Ref> constexpr decltype(auto) operator()(Ref r) const noexcept {
            return r.get();
        }
    };
    struct ptr_to_ref {
        template <class Ptr> constexpr decltype(auto) operator()(Ptr p) const noexcept {
            return *p;
        }
    };

    template <class T, class This>
    using this_to_ref = std::conditional_t<std::is_base_of<T, This>::value, //
                                           ref_to_ref,
                                           std::conditional_t<is_reference_wrapper<This>::value, //
                                                              ref_wrapper_to_ref, ptr_to_ref>>;

public:
    template <class MT, class T, class This>
    constexpr decltype(auto) operator()(MT T::*memptr, This&& this_) const noexcept {
        return this_to_ref<T, std::decay_t<This>>{}(std::forward<This>(this_)).*memptr;
    }
};

struct memptr_function_invoker_t : protected memptr_invoker_t {
    template <class MT, class T, class This, class... Args>
    constexpr decltype(auto) operator()(MT T::*memptr, This&& this_,
                                        Args&&... args) const noexcept {
        return this_to_ref<T, std::decay_t<This>>{}(std::forward<This>(this_)).*
               memptr(std::forward<Args>(args)...);
    }
};

} // namespace detail

struct invoke_t {
    template <class M, class... Args,
              std::enable_if_t<std::is_member_pointer<std::decay_t<M>>::value, bool> = true>
    constexpr decltype(auto) operator()(M memptr, Args&&... args) const noexcept(
        noexcept(std::conditional_t<std::is_function<M>::value, detail::memptr_function_invoker_t,
                                    detail::memptr_invoker_t>{}(std::declval<M>(),
                                                                std::declval<Args>()...))) {
        using invoker =
            std::conditional_t<std::is_function<M>::value, detail::memptr_function_invoker_t,
                               detail::memptr_invoker_t>;
        return invoker{}(memptr, std::forward<Args>(args)...);
    }

    template <class Fn, class... Args,
              std::enable_if_t<!std::is_member_pointer<std::decay_t<Fn>>::value, bool> = true>
    constexpr decltype(auto) operator()(Fn&& fn, Args&&... args) const
        noexcept(noexcept(std::declval<Fn>()(std::declval<Args>()...))) {
        return static_cast<Fn&&>(fn)(static_cast<Args&&>(args)...);
    }
};
constexpr invoke_t invoke{};

template <class Fn, class... Args> struct invoke_result {
    using type = decltype(invoke(std::declval<Fn>(), std::declval<Args>()...));
};
template <class Fn, class... Args>
using invoke_result_t = typename invoke_result<Fn, Args...>::type;

namespace detail {
template <class AlwaysVoid, class Fn, class... Args> struct invokable : public std::false_type {
    static constexpr bool nothrow = false;
};

template <class Fn, class... Args>
struct invokable<meta::void_t<decltype(invoke(std::declval<Fn>(), std::declval<Args>()...))>, //
                 Fn, Args...>                                                                 //
  : public std::true_type {
    static constexpr bool nothrow = noexcept(invoke(std::declval<Fn>(), std::declval<Args>()...));
};

} // namespace detail
template <class Fn, class... Args>
using is_invokable = typename detail::invokable<void, Fn, Args...>::type;
template <class Fn, class... Args> constexpr bool is_invokable_v = is_invokable<Fn, Args...>::value;

template <class Fn, class... Args>
constexpr bool is_nothrow_invokable_v = detail::invokable<void, Fn, Args...>::nothrow;
template <class Fn, class... Args>
struct is_nothrow_invokable
  : public std::integral_constant<bool, is_nothrow_invokable_v<Fn, Args...>> {};

} // namespace tmdesc
