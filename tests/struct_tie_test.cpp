//#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"
#include <tmdesc/type_description.hpp>
#include <tmdesc/tuple/tuple_operators.hpp>

namespace foo {
struct test_struct_1 {
    int v1_int;
    char v2_char;
};
struct test_struct_2 {
    test_struct_1 v1_s1;
    int v2_int;
    test_struct_1 v3_s1;
};

template <class Impl>
constexpr auto tmdesc_info(tmdesc::type_t<test_struct_1>, tmdesc::info_builder<Impl> builder) noexcept {
    using type = test_struct_1;
    auto m1 = builder.member("v1_int", &type::v1_int);
    auto m2 = builder.member("v2_char", &type::v2_char);
    (void) m1;
    (void) m2;
    return builder(m1, m2 );
}
template <class Impl>
constexpr auto tmdesc_info(tmdesc::type_t<test_struct_2>, tmdesc::info_builder<Impl> builder) noexcept {
    using type = test_struct_2;
    // Order changed
    return builder(builder.member("v1_s1", &type::v1_s1), //
                   builder.member("v3_s1", &type::v3_s1), //
                   builder.member("v2_int", &type::v2_int));
}
} // namespace foo

// include description usage header after description defenition
#include <tmdesc/struct_tie.hpp>

#define STATIC_CHECK(...)           \
    static_assert(__VA_ARGS__, ""); \
    CHECK(__VA_ARGS__)

#define STATIC_NOTHROW_CHECK(...)             \
    static_assert(__VA_ARGS__, "");           \
    static_assert(noexcept(__VA_ARGS__), ""); \
    CHECK(__VA_ARGS__)

#define NOTHROW_CHECK(...)             \
    static_assert(noexcept(__VA_ARGS__), ""); \
    CHECK(__VA_ARGS__)

template <class E>struct  check_type;

struct ignore{
template<class T>
constexpr bool operator()(const T&)const noexcept{ return false;}
};

TEST_CASE("tuple initialisation and get") {
    SUBCASE("simple struct_tie") {
        static constexpr foo::test_struct_1 s1{42, char(-42)};
        static constexpr auto tie = tmdesc::struct_tie(s1);
        using tie_type = tmdesc::meta::remove_cvref_t<decltype (tie)>;

        static_assert (std::is_same<tie_type, tmdesc::tuple<const int&, const char&>>{}, "");

        STATIC_NOTHROW_CHECK(s1.v1_int == tmdesc::get<0>(tie));
        STATIC_NOTHROW_CHECK(s1.v2_char == tmdesc::get<1>(tie));
    }
    SUBCASE("mutable struct_tie") {
        foo::test_struct_1 s1{42, char(-42)};
        auto tie = tmdesc::struct_tie(s1);
        using tie_type = tmdesc::meta::remove_cvref_t<decltype (tie)>;

        static_assert (std::is_same<tie_type, tmdesc::tuple<int&, char&>>{}, "");

        NOTHROW_CHECK(s1.v1_int == tmdesc::get<0>(tie));
        NOTHROW_CHECK(s1.v2_char == tmdesc::get<1>(tie));

        tmdesc::get<0>(tie) = -21;
        s1.v2_char = 21;

        NOTHROW_CHECK(s1.v1_int == tmdesc::get<0>(tie));
        NOTHROW_CHECK(s1.v2_char == tmdesc::get<1>(tie));
        NOTHROW_CHECK(s1.v1_int == -21);
        NOTHROW_CHECK(s1.v2_char == 21);
    }
    SUBCASE("complex struct_tie") {
        static constexpr foo::test_struct_2 s2{{42, char(-42)},555, {-21, char(21)}};
        static constexpr auto tie = tmdesc::struct_tie(s2);
        using tie_type = tmdesc::meta::remove_cvref_t<decltype (tie)>;

        static_assert (std::is_same<tie_type, tmdesc::tuple<const foo::test_struct_1&, const foo::test_struct_1&, const int&>>{}, "");
        static_assert (noexcept (tmdesc::tuple_transform(tmdesc::detail::type_info_v<foo::test_struct_2>.members(), ignore{}) ), "");
        static_assert (noexcept (tmdesc::struct_tie(s2)), "");
        STATIC_NOTHROW_CHECK(tmdesc::struct_tie(s2.v3_s1) == tmdesc::struct_tie(tmdesc::get<1>(tie)));
        STATIC_NOTHROW_CHECK(s2.v2_int == tmdesc::get<2>(tie));
    }
}
