// Copyright Victor Smirnov 2021
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
//
// The documentation can be found at the library's page:
// https://github.com/Ariox41/tmdesc

#pragma once
#include <type_traits>
namespace tmdesc {
template <class DataType, class Enable = void> struct make_impl {
    template <class... Args>
    static constexpr auto apply(Args&&... args) noexcept(noexcept(DataType{std::declval<Args>()...}))
        -> decltype(DataType{std::declval<Args>()...}) {
        return DataType{static_cast<Args&&>(args)...};
    }
};

template <class T> struct make_t {
    using impl_t = make_impl<std::decay_t<T>>;

    template <class... Args>
    constexpr auto operator()(Args&&... args) const noexcept(    //
        noexcept(impl_t::apply(std::declval<Args>()...)))     //
        -> decltype(impl_t::apply(std::declval<Args>()...)) { //
        return impl_t::apply(static_cast<Args&&>(args)...);   //
    }
};

/// Create an object of the tag or type `T` with given arguments
///
/// @details
/// If T is a "normal" type, the function simply calls the constructor with arguments.
/// If T is a tag, If T is a tag, creates an instance of the type associated with the tag
///
/// @warning
/// If T is a tag, but no make implementation is defined for it,
/// the function will create an instance of the tag, which is wrong.
template <class T> constexpr make_t<T> make{};

} // namespace tmdesc
