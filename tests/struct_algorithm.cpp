
//#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"
#include <string>
#include <tmdesc/algorithm/for_each.hpp>
#include <tmdesc/struct_as_tuple.hpp>

#define STATIC_CHECK(...)           \
    static_assert(__VA_ARGS__, ""); \
    CHECK(__VA_ARGS__)

#define STATIC_NOTHROW_CHECK(...)             \
    static_assert(__VA_ARGS__, "");           \
    static_assert(noexcept(__VA_ARGS__), ""); \
    CHECK(__VA_ARGS__)

namespace ns {
struct s1 {
    int m0;
    char m1;
    unsigned int m2;
};

template <class Impl> constexpr auto tmdesc_info(tmdesc::type_t<s1>, tmdesc::info_builder<Impl> builder) {
    return builder(builder.member("m0", &s1::m0), builder.member("m1", &s1::m1), builder.member("m2", &s1::m2));
}
} // namespace ns

struct add {
    int& acc;
    template <class T> constexpr void operator()(const T& value) const noexcept { acc += int(value); }
};

template <class T> constexpr int calc_member_sum(const T& t) noexcept {
    int acc = 0;
    ::tmdesc::for_each(t, add{acc});
    return acc;
}

TEST_CASE("tuple algorithm for struct") {
    SUBCASE("simple get") {
        constexpr ns::s1 s1{41, '3', 56};

        STATIC_NOTHROW_CHECK(41 == ::tmdesc::get<0>(s1));
        STATIC_NOTHROW_CHECK('3' == ::tmdesc::get<1>(s1));
        STATIC_NOTHROW_CHECK(56 == ::tmdesc::get<2>(s1));
    }
    SUBCASE("tuple foreach") {
        constexpr ns::s1 s1{41, '3', 56};
        STATIC_NOTHROW_CHECK(calc_member_sum(s1) == (41 + '3' + 56));
    }
}
