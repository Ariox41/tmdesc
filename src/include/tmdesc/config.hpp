#pragma once

// NOTE The tmdesc implementation for C++14 is fully supported only for gcc and clang.

#ifdef __has_include
#if __has_include(<version>)
#include <version>
#endif
#endif

#if __cplusplus < 201400
#error "C++14 or better is required"
#endif

#ifdef __cpp_lib_string_view
#if __cpp_lib_string_view >= 201606L
#define TMDESC_CONFIG_ENABLE_STD_STRING_VIEW
#endif
#endif

#ifdef __cpp_lib_is_invocable
#if __cpp_lib_is_invocable >= 201703L
#define TMDESC_CONFIG_ENABLE_STD_INVOCABLE
#endif
#endif

#ifdef __cpp_inline_variables
#if __cpp_inline_variables >= 201606L
#define TMDESC_INLINE_VARIABLES inline
#else
#define TMDESC_INLINE_VARIABLES /*nothing*/
#endif
#else
#define TMDESC_INLINE_VARIABLES /*nothing*/
#endif
