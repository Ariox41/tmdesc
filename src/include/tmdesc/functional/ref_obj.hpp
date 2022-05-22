// Copyright Victor Smirnov 2021-2022
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
//
// The documentation can be found at the library's page:
// https://github.com/Ariox41/tmdesc

#pragma once
// for forward declaration of reference_wrapper and for decay_t
#include <type_traits>

namespace tmdesc {

/** Represents reference as object with address and copy constructor/assignment.
 * @details
 * Like `std::reference_wrapper`, `ref_obj` allows you to copy references as objects.
 * Unlike std::reference_wrapper, the template argument of `ref_obj` contains the reference type, not the object type,
 * and `ref_obj` supports both lvalue and rvalue references.
 * @note ref_obj is full constexpr type
 * @see ref_cast for explicit conversions
 * @see pointer_to_ref for explicit pointer dereference
 */
template <class R> struct ref_obj {
    static_assert(std::is_reference<R>{},
                  "the template argument of `ref_obj` contains the reference type, not the object type");

    using type = std::remove_reference_t<R>;

    ref_obj() = delete;

    constexpr ref_obj(R&& r) noexcept
      : ptr_(&r) {}
    constexpr ref_obj(const ref_obj&) noexcept = default;

    constexpr ref_obj& operator=(const ref_obj& x) noexcept = default;

    constexpr operator R() const noexcept { return get(); }
    constexpr R get() const noexcept { return static_cast<R>(*ptr_); }

private:
    type* ptr_;
};

struct ref_t {
    template <class T> constexpr ref_obj<T&&> operator()(T&& v) const noexcept { return {static_cast<T&&>(v)}; }
    template <class T> constexpr ref_obj<T> operator()(ref_obj<T> v) const noexcept { return v; }
};
constexpr ref_t ref;

struct cref_t {
    template <class T> constexpr ref_obj<const T&> operator()(T&& v) const noexcept { return {v}; }
    template <class T> constexpr ref_obj<const T&> operator()(ref_obj<T> v) const noexcept { return v; }
};
constexpr cref_t cref;

template <class T> struct is_ref_obj : std::false_type {};
template <class U> struct is_ref_obj<ref_obj<U>> : std::true_type {};
template <class T> constexpr bool is_ref_obj_v = is_ref_obj<T>::value;

template <class To> struct ref_cast_fn {
    template <class From, std::enable_if_t<std::is_convertible<From&, To&>::value, bool> = true>
    constexpr ref_obj<To&> operator()(ref_obj<From&> from) const noexcept {
        return {from.get()};
    }
    template <class From, std::enable_if_t<std::is_convertible<From&&, To&&>::value, bool> = true>
    constexpr ref_obj<To&&> operator()(ref_obj<From&&> from) const noexcept {
        return {from.get()};
    }
    template <class From, std::enable_if_t<std::is_convertible<const From&, const To&>::value, bool> = true>
    constexpr ref_obj<const To&> operator()(ref_obj<const From&> from) const noexcept {
        return {from.get()};
    }
};

/// Functional object for explicit cast `ref_obj<From&>` to `ref_obj<To&>`
/// or `ref_obj<From&&>` to `ref_obj<To&&>`
/// or `ref_obj<const From&>` to `ref_obj<const To&>`
template <class To> constexpr ref_cast_fn<To> ref_cast{};

struct pointer_to_ref_fn {
    template <class T>
    constexpr auto operator()(T& ptr) const noexcept(noexcept(*std::declval<T&>()))
        -> ref_obj<decltype(*std::decay_t<T&>())> {
        return {*ptr};
    }
};

/// Functional object for explicit dereference pointer-like object  to `ref_obj`
/// @note `ref_obj<Pointer>` is converted to `ref_obj<deref<Pointer>>`
/// @warning In the case of proxy objects, the link may be invalid
constexpr pointer_to_ref_fn pointer_to_ref{};

} // namespace tmdesc
