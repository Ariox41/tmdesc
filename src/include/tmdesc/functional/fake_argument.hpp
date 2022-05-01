// Copyright Victor Smirnov 2021
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
//
// The documentation can be found at the library's page:
// https://github.com/Ariox41/tmdesc

#pragma once

namespace tmdesc {
/// An argument type indicating that the function must take a value of any type.
///
/// @details 
/// A function without arguments is instantiated even if it is never called.
/// To avoid this, you need to make the function a template.
/// When calling such functions, `fake_argument` is passed as an argument.
///
/// @see eval
///
/// @warning 
/// The function that accepts this argument must be a template, without any specialization for `fake_argument_type`.
/// For example, it can be a generic lambda.
struct fake_argument_type {};
constexpr fake_argument_type fake_argument{};

} // namespace tmdesc
