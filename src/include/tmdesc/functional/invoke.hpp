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
            return (static_cast<T1&&>(t1).*f)(static_cast<Args&&>(args)...);
        else if constexpr (tmdesc::is_ref_obj_v<std::decay_t<T1>>)
            return (t1.get().*f)(static_cast<Args&&>(args)...);
        else
            return ((*static_cast<T1&&>(t1)).*f)(static_cast<Args&&>(args)...);
    } else {
        static_assert(std::is_object_v<Pointed> && sizeof...(args) == 0);
        if constexpr (std::is_base_of_v<C, std::decay_t<T1>>)
            return static_cast<T1&&>(t1).*f;
        else if constexpr (tmdesc::is_ref_obj_v<std::decay_t<T1>>)
            return t1.get().*f;
        else
            return (*static_cast<T1&&>(t1)).*f;
    }
}

} // namespace detail

struct invoke_t {
    template <class F, class... Args>
    constexpr invoke_result_t<F, Args...> operator()(F&& f, Args&&... args) const
        noexcept(is_nothrow_invocable_v<F, Args...>) {
        if constexpr (std::is_member_pointer_v<std::decay_t<F>>)
            return tmdesc::detail::invoke_memptr(f, static_cast<Args&&>(args)...);
        else
            return static_cast<F&&>(f)(static_cast<Args&&>(args)...);
    }
};
constexpr invoke_t invoke{};
} // namespace tmdesc
#else
namespace tmdesc {

namespace detail {

// pointer to member helper
struct invoke_pmd_helper {
    struct pass_helper {
        using type = pass_helper;
        template <class Ref> static constexpr Ref&& apply(Ref&& r) noexcept { return static_cast<Ref&&>(r); }
    };
    struct call_get_helper {
        using type = call_get_helper;
        template <class Ref> static constexpr auto apply(Ref r) noexcept -> decltype(std::declval<Ref&>().get()) {
            return r.get();
        }
    };
    struct deref_helper {
        using type = deref_helper;
        template <class Ptr> static constexpr auto apply(Ptr p) noexcept -> decltype(*std::declval<Ptr&>()) {
            return *p;
        }
    };

    template <class Self, class SelfRef>
    using accessor_helper = typename std::conditional_t<std::is_base_of<Self, SelfRef>::value,       //
                                                        pass_helper,                                 //
                                                        std::conditional<is_ref_obj<SelfRef>::value, //
                                                                         call_get_helper,            //
                                                                         deref_helper                //
                                                                         >>::type;

    template <class MT, class T, class This>
    static constexpr auto apply_(MT T::*pmd, This&& this_) noexcept(noexcept(static_cast<This&&>(this_).*pmd))
        -> decltype(static_cast<This&&>(this_).*pmd) {
        return static_cast<This&&>(this_).*pmd;
    }

    template <class MT, class T, class This>
    static constexpr auto apply(MT T::*pmd, This&& this_) noexcept(
        noexcept(apply_(pmd, accessor_helper<T, std::decay_t<This>>::apply(static_cast<This&&>(this_)))))
        -> decltype(apply_(pmd, accessor_helper<T, std::decay_t<This>>::apply(static_cast<This&&>(this_)))) {
        return apply_(pmd, accessor_helper<T, std::decay_t<This>>::apply(static_cast<This&&>(this_)));
    }
};

// pointer to member function helper
struct invoke_pmf_helper {
    template <class Self, class SelfRef> using accessor_helper = invoke_pmd_helper::accessor_helper<Self, SelfRef>;

    template <class R, class T, class This, class... Args>
    static constexpr auto apply(R T::*memptr, This&& this_, Args&&... args)                              //
        noexcept(noexcept((accessor_helper<T, std::decay_t<This>>::apply(std::declval<This>()).*memptr)( //
            std::declval<Args>()...                                                                      //
            )))                                                                                          //
        -> decltype((accessor_helper<T, std::decay_t<This>>::apply(std::declval<This>()).*memptr)(       //
            std::declval<Args>()...                                                                      //
            )) {

        return                                                                                  //
            (accessor_helper<T, std::decay_t<This>>::apply(std::forward<This>(this_)).*memptr)( //
                std::forward<Args>(args)...                                                     //
            );
    }
};

struct call_helper {
    using type = call_helper;
    template <class Fn, class... Args>
    static constexpr auto apply(Fn&& fn, Args&&... args) //
        noexcept(noexcept(std::declval<Fn>()(std::declval<Args>()...)))
            -> decltype(std::declval<Fn>()(std::declval<Args>()...)) {
        return static_cast<Fn&&>(fn)(static_cast<Args&&>(args)...);
    }
};

template <class M>
struct get_invoke_pm_helper
  : std::conditional<std::is_member_function_pointer<M>::value, detail::invoke_pmf_helper, detail::invoke_pmd_helper> {
};

template <class Fn>
using invoke_helper = typename std::conditional_t<std::is_member_pointer<std::decay_t<Fn>>::value,
                                                  get_invoke_pm_helper<std::decay_t<Fn>>, call_helper>::type;

template <class Void, class Fn, class... Args> struct invoke_result_impl;
template <class Fn, class... Args>
struct invoke_result_impl<decltype(void(detail::invoke_helper<Fn>::apply(std::declval<Fn>(), std::declval<Args>()...))),
                          Fn, Args...> {
    using type = decltype(detail::invoke_helper<Fn>::apply(std::declval<Fn>(), std::declval<Args>()...));
};

} // namespace detail

struct invoke_t {
    template <class Fn, class... Args>
    constexpr auto operator()(Fn&& fn, Args&&... args) const
        noexcept(noexcept(detail::invoke_helper<Fn>::apply(std::declval<Fn>(), std::declval<Args>()...)))
            -> decltype(detail::invoke_helper<Fn>::apply(std::declval<Fn>(), std::declval<Args>()...)) {
        return detail::invoke_helper<Fn>::apply(static_cast<Fn&&>(fn), static_cast<Args&&>(args)...);
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
