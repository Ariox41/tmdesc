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
template <class Fn, class = void> struct invoke_impl {
    template <class Callable, class... Args>
    static constexpr auto apply(Callable&& fn,
                                Args&&... args) noexcept(noexcept(std::declval<Callable>()(std::declval<Args>()...)))
        -> decltype(std::declval<Callable>()(std::declval<Args>()...)) {
        return static_cast<Callable&&>(fn)(static_cast<Args&&>(args)...);
    }
};

struct invoke_t {
    template <class Fn, class... Args>
    constexpr auto operator()(Fn&& fn, Args&&... args) const
        noexcept(noexcept(invoke_impl<std::decay_t<Fn>>::apply(std::declval<Fn>(), std::declval<Args>()...)))
            -> decltype(invoke_impl<std::decay_t<Fn>>::apply(std::declval<Fn>(), std::declval<Args>()...)) {
        return invoke_impl<std::decay_t<Fn>>::apply(static_cast<Fn&&>(fn), static_cast<Args&&>(args)...);
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

template <class Fn, class... Args> using is_invocable = typename ::tmdesc::detail::invocable<Fn(Args...)>::type;
template <class Fn, class... Args> constexpr bool is_invocable_v = is_invocable<Fn, Args...>::value;

template <class Fn, class... Args>
constexpr bool is_nothrow_invocable_v = ::tmdesc::detail::invocable<Fn(Args...)>::nothrow;
template <class Fn, class... Args>
struct is_nothrow_invocable : public std::integral_constant<bool, is_nothrow_invocable_v<Fn, Args...>> {};
namespace detail {

struct invoke_member_object_pointer_impl {
protected:
    struct ref_to_ref_impl {
        template <class Ref> static constexpr Ref&& apply(Ref&& r) noexcept { return static_cast<Ref&&>(r); }
    };
    struct ref_obj_to_ref_impl {
        template <class Ref> static constexpr auto apply(Ref r) noexcept -> decltype(std::declval<Ref&>().get()) {
            return r.get();
        }
    };
    struct ptr_to_ref_impl {
        template <class Ptr> static constexpr auto apply(Ptr p) noexcept -> decltype(*std::declval<Ptr&>()) {
            return *p;
        }
    };

    template <class RealThisType, class CurrentThisType>
    using this_to_ref_impl = std::conditional_t<std::is_base_of<RealThisType, CurrentThisType>{}, //
                                                ref_to_ref_impl,
                                                std::conditional_t<is_ref_obj<CurrentThisType>{}, //
                                                                   ref_obj_to_ref_impl, ptr_to_ref_impl>>;

public:
    template <class MT, class T, class This>
    static constexpr auto
    apply(MT T::*memptr,
          This&& this_) noexcept(noexcept(this_to_ref_impl<T, std::decay_t<This>>::apply(std::declval<This>()).*memptr))
        -> decltype(this_to_ref_impl<T, std::decay_t<This>>::apply(std::declval<This>()).*memptr) {
        return this_to_ref_impl<T, std::decay_t<This>>::apply(std::forward<This>(this_)).*memptr;
    }
};

template <class mfn> struct mem_fn_owner_type;
template <class R, class T, class... Args> struct mem_fn_owner_type<R (T::*)(Args...)> { using type = T; };
template <class R, class T, class... Args> struct mem_fn_owner_type<R (T::*)(Args...)&> { using type = T; };
template <class R, class T, class... Args> struct mem_fn_owner_type<R (T::*)(Args...) &&> { using type = T; };
template <class R, class T, class... Args> struct mem_fn_owner_type<R (T::*)(Args...) const> { using type = T; };
template <class R, class T, class... Args> struct mem_fn_owner_type<R (T::*)(Args...) const&> { using type = T; };
template <class R, class T, class... Args> struct mem_fn_owner_type<R (T::*)(Args...) const&&> { using type = T; };

struct invoke_member_function_pointer_impl : private invoke_member_object_pointer_impl {
    template <class Mfn, class This, class... Args>
    static constexpr auto apply(Mfn memptr, This&& this_, Args&&... args) //
        noexcept(noexcept((this_to_ref_impl<typename mem_fn_owner_type<std::decay_t<Mfn>>::type,
                                            std::decay_t<This>>::apply(std::declval<This>()) //
                           .*memptr)                                                         //
                          (std::declval<Args>()...)))
            -> decltype((this_to_ref_impl<typename mem_fn_owner_type<std::decay_t<Mfn>>::type,
                                          std::decay_t<This>>::apply(std::declval<This>()) //
                         .*memptr)                                                         //
                        (std::declval<Args>()...)) {

        return //
            (this_to_ref_impl<typename mem_fn_owner_type<std::decay_t<Mfn>>::type, std::decay_t<This>>::apply(
                 std::forward<This>(this_)) //
             .*memptr)                      //
            (std::forward<Args>(args)...);
    }
};

} // namespace detail

template <class Fn>
struct invoke_impl<Fn, std::enable_if_t<std::is_member_pointer<Fn>{}>>
  : std::conditional_t<std::is_member_function_pointer<Fn>{}, ::tmdesc::detail::invoke_member_function_pointer_impl,
                       ::tmdesc::detail::invoke_member_object_pointer_impl> {};
} // namespace tmdesc
