#include <doctest/doctest.h>

#define STATIC_CHECK(...) static_assert(__VA_ARGS__, "");

//   CHECK(__VA_ARGS__);

#define STATIC_NOTHROW_CHECK(...)   \
    static_assert(__VA_ARGS__, ""); \
    static_assert(noexcept(__VA_ARGS__), "");

// CHECK(__VA_ARGS__)