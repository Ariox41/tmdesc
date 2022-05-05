#include <doctest/doctest.h>

#define STATIC_CHECK(...) static_assert(__VA_ARGS__, "");

//   CHECK(__VA_ARGS__);

#define STATIC_CHECK_AND_POSSIBLE_NOTHROW(...)   \
    static_assert(__VA_ARGS__, ""); 
// boost::hana does not support noexcept
//   static_assert(noexcept(__VA_ARGS__), "");

// CHECK(__VA_ARGS__)