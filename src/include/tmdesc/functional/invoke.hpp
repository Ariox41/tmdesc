// Copyright Victor Smirnov 2023
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
//
// The documentation can be found at the library's page:
// https://github.com/Ariox41/tmdesc

#pragma once
#include <tmdesc/config.hpp>
#include <tmdesc/functional/ref_obj_fwd.hpp>
#include <type_traits>
#include <utility>

#ifdef TMDESC_CONFIG_ENABLE_STD_INVOCABLE
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
#else
namespace tmdesc {

namespace detail {

//// see std::void_t
//template <class... Ts> struct make_void { typedef void type; };
// template <class... Ts> using void_t = typename make_void<Ts...>::type;

struct invoke_pmd_fn {
    struct pass_fn {
        template <class Ref> constexpr Ref&& operator()(Ref&& r) const noexcept { return static_cast<Ref&&>(r); }
    };
    struct call_get_fn {
        template <class Ref> constexpr auto operator()(Ref r) const noexcept -> decltype(std::declval<Ref&>().get()) {
            return r.get();
        }
    };
    struct deref_fn {
        template <class Ptr> constexpr auto operator()(Ptr p) const noexcept -> decltype(*std::declval<Ptr&>()) {
            return *p;
        }
    };

    template <class Self, class SelfRef>
    using accessor_fn = std::conditional_t<std::is_base_of<Self, SelfRef>::value,         //
                                           pass_fn,                                       //
                                           std::conditional_t<is_ref_obj<SelfRef>::value, //
                                                              call_get_fn,                //
                                                              deref_fn                    //
                                                              >>;

public:
    template <class MT, class T, class This>
    constexpr auto operator()(MT T::*memptr, This&& this_) const
        noexcept(noexcept(accessor_fn<T, std::decay_t<This>>{}(std::declval<This>()).*memptr))
            -> decltype(accessor_fn<T, std::decay_t<This>>{}(std::declval<This>()).*memptr) {
        return accessor_fn<T, std::decay_t<This>>{}(std::forward<This>(this_)).*memptr;
    }
};

struct invoke_pmf_fn : private invoke_pmd_fn {
    template <class R, class T, class This, class... Args>
    constexpr auto operator()(R T::*memptr, This&& this_, Args&&... args) const //
        noexcept(noexcept((accessor_fn<T, std::decay_t<This>>{}(std::declval<This>()).*
                           memptr)(std::declval<Args>()...)))
            -> decltype((accessor_fn<T, std::decay_t<This>>{}(std::declval<This>()).*memptr)(std::declval<Args>()...)) {

        return //
            (accessor_fn<T, std::decay_t<This>>{}(std::forward<This>(this_)).*memptr)(std::forward<Args>(args)...);
    }
};

template <class M>
using invoke_pm_fn =
    std::conditional<std::is_member_function_pointer<M>::value, detail::invoke_pmf_fn, detail::invoke_pmd_fn>;

struct call_fn {
    using type = call_fn;
    template <class Fn, class... Args>
    constexpr auto operator()(Fn&& fn, Args&&... args) const
        noexcept(noexcept(std::declval<Fn>()(std::declval<Args>()...)))
            -> decltype(std::declval<Fn>()(std::declval<Args>()...)) {
        return static_cast<Fn&&>(fn)(static_cast<Args&&>(args)...);
    }
};

template <class Fn>
using invoke_fn =
    typename std::conditional_t<std::is_member_pointer<std::decay_t<Fn>>::value, invoke_pm_fn<Fn>, call_fn>::type;

template <class Void, class Fn, class... Args> struct invoke_result_impl;
template <class Fn, class... Args>
struct invoke_result_impl<decltype(void(detail::invoke_fn<Fn>{}(std::declval<Fn>(), std::declval<Args>()...))), Fn,
                          Args...> {
    using type = decltype(detail::invoke_fn<Fn>{}(std::declval<Fn>(), std::declval<Args>()...));
};

} // namespace detail

struct invoke_t {
    template <class Fn, class... Args>
    constexpr auto operator()(Fn&& fn, Args&&... args) const
        noexcept(noexcept(detail::invoke_fn<Fn>{}(std::declval<Fn>(), std::declval<Args>()...)))
            -> decltype(detail::invoke_fn<Fn>{}(std::declval<Fn>(), std::declval<Args>()...)) {
        return detail::invoke_fn<Fn>{}(static_cast<Fn&&>(fn), static_cast<Args&&>(args)...);
    }
};
constexpr invoke_t invoke{};

// template <class Fn, class... Args> struct invoke_result {
//    using type = decltype(detail::invoke_fn<Fn>{}(std::declval<Fn>(), std::declval<Args>()...));
//};
template <class Fn, class... Args> struct invoke_result : detail::invoke_result_impl<void, Fn, Args...> {};
template <class Fn, class... Args> using invoke_result_t = typename invoke_result<Fn, Args...>::type;

namespace detail {
template <class Fn, class = void> struct invocable {
    using type                    = std::false_type;
    static constexpr bool nothrow = false;
};

template <class Fn, class... Args>
struct invocable<Fn(Args...), decltype(void(invoke(std::declval<Fn>(), std::declval<Args>()...)))> {
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
#endif
