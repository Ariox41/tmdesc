// Copyright Victor Smirnov 2021-2022
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
//
// The documentation can be found at the library's page:
// https://github.com/Ariox41/tmdesc

#pragma once
#include "../meta/void_t.hpp"
#include "ref_obj.hpp"
#include <utility>

namespace tmdesc {
namespace detail {

struct memptr_invoker_fn {
protected:
    struct ref_to_ref_fn {
        template <class Ref> constexpr Ref&& operator()(Ref&& r) const noexcept { return static_cast<Ref&&>(r); }
    };
    struct ref_obj_to_ref_fn {
        template <class Ref> constexpr auto operator()(Ref r) const noexcept -> decltype(std::declval<Ref&>().get()) {
            return r.get();
        }
    };
    struct ptr_to_ref_fn {
        template <class Ptr> constexpr auto operator()(Ptr p) const noexcept -> decltype(*std::declval<Ptr&>()) {
            return *p;
        }
    };

    template <class RealThisType, class CurrentThisType>
    using this_to_ref_fn = std::conditional_t<std::is_base_of<RealThisType, CurrentThisType>::value, //
                                              ref_to_ref_fn,
                                              std::conditional_t<is_ref_obj<CurrentThisType>::value, //
                                                                 ref_obj_to_ref_fn, ptr_to_ref_fn>>;

public:
    template <class MT, class T, class This>
    constexpr auto operator()(MT T::*memptr, This&& this_) const
        noexcept(noexcept(this_to_ref_fn<T, std::decay_t<This>>{}(std::declval<This>()).*memptr))
            -> decltype(this_to_ref_fn<T, std::decay_t<This>>{}(std::declval<This>()).*memptr) {
        return this_to_ref_fn<T, std::decay_t<This>>{}(std::forward<This>(this_)).*memptr;
    }
};

template <class mfn> struct mem_fn_owner_type;
template <class R, class T, class... Args> struct mem_fn_owner_type<R (T::*)(Args...)> { using type = T; };
template <class R, class T, class... Args> struct mem_fn_owner_type<R (T::*)(Args...)&> { using type = T; };
template <class R, class T, class... Args> struct mem_fn_owner_type<R (T::*)(Args...) &&> { using type = T; };
template <class R, class T, class... Args> struct mem_fn_owner_type<R (T::*)(Args...) const> { using type = T; };
template <class R, class T, class... Args> struct mem_fn_owner_type<R (T::*)(Args...) const&> { using type = T; };
template <class R, class T, class... Args> struct mem_fn_owner_type<R (T::*)(Args...) const&&> { using type = T; };

struct memptr_function_invoker_t : private memptr_invoker_fn {
    template <class Mfn, class This, class... Args>
    constexpr auto operator()(Mfn memptr, This&& this_, Args&&... args) const //
        noexcept(noexcept((this_to_ref_fn<typename mem_fn_owner_type<std::decay_t<Mfn>>::type, std::decay_t<This>>{}(
                               std::declval<This>()) //
                           .*memptr)                 //
                          (std::declval<Args>()...)))
            -> decltype((this_to_ref_fn<typename mem_fn_owner_type<std::decay_t<Mfn>>::type, std::decay_t<This>>{}(
                             std::declval<This>()) //
                         .*memptr)                 //
                        (std::declval<Args>()...)) {

        return //
            (this_to_ref_fn<typename mem_fn_owner_type<std::decay_t<Mfn>>::type, std::decay_t<This>>{}(
                 std::forward<This>(this_)) //
             .*memptr)                      //
            (std::forward<Args>(args)...);
    }
};

} // namespace detail

struct invoke_t {
    template <class M, class... Args, std::enable_if_t<std::is_member_pointer<std::decay_t<M>>::value, bool> = true>
    constexpr auto operator()(M memptr, Args&&... args) const noexcept(
        noexcept(std::conditional_t<std::is_member_function_pointer<M>::value, detail::memptr_function_invoker_t,
                                    detail::memptr_invoker_fn>{}(std::declval<M>(), std::declval<Args>()...)))
        -> decltype(std::conditional_t<std::is_member_function_pointer<M>::value, detail::memptr_function_invoker_t,
                                       detail::memptr_invoker_fn>{}(std::declval<M>(), std::declval<Args>()...)) {
        return std::conditional_t<std::is_member_function_pointer<M>::value, detail::memptr_function_invoker_t,
                                  detail::memptr_invoker_fn>{}(memptr, std::forward<Args>(args)...);
    }

    template <class Fn, class... Args, std::enable_if_t<!std::is_member_pointer<std::decay_t<Fn>>::value, bool> = true>
    constexpr auto operator()(Fn&& fn, Args&&... args) const
        noexcept(noexcept(std::declval<Fn>()(std::declval<Args>()...)))
            -> decltype(std::declval<Fn>()(std::declval<Args>()...)) {
        return static_cast<Fn&&>(fn)(static_cast<Args&&>(args)...);
    }
};
constexpr invoke_t invoke{};

template <class Fn, class... Args> struct invoke_result {
    using type = decltype(invoke(std::declval<Fn>(), std::declval<Args>()...));
};
template <class Fn, class... Args> using invoke_result_t = typename invoke_result<Fn, Args...>::type;

namespace detail {
template <class Fn, class = void> struct invocable {
    using type                    = std::false_type;
    static constexpr bool nothrow = false;
};

template <class Fn, class... Args>
struct invocable<Fn(Args...), meta::void_t<decltype(invoke(std::declval<Fn>(), std::declval<Args>()...))>> {
    using type                    = std::true_type;
    static constexpr bool nothrow = noexcept(invoke(std::declval<Fn>(), std::declval<Args>()...));
};

} // namespace detail

template <class Fn, class... Args> using is_invocable            = typename detail::invocable<Fn(Args...)>::type;
template <class Fn, class... Args> constexpr bool is_invocable_v = is_invocable<Fn, Args...>::value;

template <class Fn, class... Args> constexpr bool is_nothrow_invocable_v = detail::invocable<Fn(Args...)>::nothrow;
template <class Fn, class... Args>
struct is_nothrow_invocable : public std::integral_constant<bool, is_nothrow_invocable_v<Fn, Args...>> {};
} // namespace tmdesc
