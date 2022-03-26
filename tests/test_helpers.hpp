#include <doctest/doctest.h>

#define STATIC_CHECK(...) \
    static_assert (__VA_ARGS__, ""); \
    CHECK(__VA_ARGS__);
