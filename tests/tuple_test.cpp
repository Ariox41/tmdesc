#include <doctest/doctest.h>

#include <string>
#include <tmdesc/tuple/tuple.hpp>
#include <tmdesc/tuple/tuple_operators.hpp>
#include <tmdesc/algorithm/for_each.hpp>
#include <tmdesc/algorithm/transform.hpp>
#include <tmdesc/string_view.hpp>

static_assert(std::is_trivial<tmdesc::tuple<>>{}, "");
static_assert(std::is_trivial<tmdesc::tuple<int>>{}, "");
static_assert(std::is_trivial<tmdesc::tuple<int, char>>{}, "");

static_assert(std::is_trivially_copyable<tmdesc::tuple<>>{}, "");
static_assert(std::is_trivially_copyable<tmdesc::tuple<int>>{}, "");
static_assert(std::is_trivially_copyable<tmdesc::tuple<int, char>>{}, "");

static_assert(std::is_trivially_copy_constructible<tmdesc::tuple<>>{}, "");
static_assert(std::is_trivially_copy_constructible<tmdesc::tuple<int>>{}, "");
static_assert(std::is_trivially_copy_constructible<tmdesc::tuple<int, char>>{}, "");

static_assert(std::is_trivially_copy_assignable<tmdesc::tuple<>>{}, "");
static_assert(std::is_trivially_copy_assignable<tmdesc::tuple<int>>{}, "");
static_assert(std::is_trivially_copy_assignable<tmdesc::tuple<int, char>>{}, "");

static_assert(std::is_trivially_move_assignable<tmdesc::tuple<>>{}, "");
static_assert(std::is_trivially_move_assignable<tmdesc::tuple<int>>{}, "");
static_assert(std::is_trivially_move_assignable<tmdesc::tuple<int, char>>{}, "");

static_assert(std::is_trivially_destructible<tmdesc::tuple<>>{}, "");
static_assert(std::is_trivially_destructible<tmdesc::tuple<int>>{}, "");
static_assert(std::is_trivially_destructible<tmdesc::tuple<int, char>>{}, "");

static_assert(std::is_trivially_destructible<tmdesc::tuple<>>{}, "");
static_assert(std::is_trivially_destructible<tmdesc::tuple<int>>{}, "");
static_assert(std::is_trivially_destructible<tmdesc::tuple<int, char>>{}, "");

static_assert(std::is_nothrow_default_constructible<tmdesc::tuple<>>{}, "");
static_assert(std::is_nothrow_default_constructible<tmdesc::tuple<int>>{}, "");
static_assert(std::is_nothrow_default_constructible<tmdesc::tuple<int, char>>{}, "");

static_assert(std::is_nothrow_copy_assignable<tmdesc::tuple<>>{}, "");
static_assert(std::is_nothrow_copy_assignable<tmdesc::tuple<int>>{}, "");
static_assert(std::is_nothrow_copy_assignable<tmdesc::tuple<int, char>>{}, "");

static_assert(!std::is_trivial<tmdesc::tuple<std::string>>{}, "");
static_assert(!std::is_trivially_destructible<tmdesc::tuple<std::string>>{}, "");
static_assert(!std::is_nothrow_copy_constructible<tmdesc::tuple<std::string>>{}, "");
static_assert(!std::is_nothrow_copy_assignable<tmdesc::tuple<std::string>>{}, "");
static_assert(std::is_copy_constructible<tmdesc::tuple<std::string>>{}, "");
static_assert(std::is_copy_assignable<tmdesc::tuple<std::string>>{}, "");

static_assert(!std::is_trivial<tmdesc::tuple<std::string, std::string>>{}, "");
static_assert(!std::is_trivially_destructible<tmdesc::tuple<std::string, std::string>>{}, "");
static_assert(!std::is_nothrow_copy_constructible<tmdesc::tuple<std::string, std::string>>{}, "");
static_assert(!std::is_nothrow_copy_assignable<tmdesc::tuple<std::string, std::string>>{}, "");
static_assert(std::is_copy_constructible<tmdesc::tuple<std::string, std::string>>{}, "");
static_assert(std::is_copy_assignable<tmdesc::tuple<std::string, std::string>>{}, "");

struct nothrow_visitor{
    template<class T> constexpr bool operator() (const T&)const noexcept{ return false;}
};
struct throw_visitor{
    template <class T> constexpr bool operator()(const T&) const noexcept(false) {
        throw "";
        return false;
    }
};

//static_assert (!noexcept (tmdesc::repeat_n<4>(throw_visitor{})), "");

static_assert (noexcept (tmdesc::for_each(tmdesc::make_tuple(1, '2', "42"), nothrow_visitor{})), "");
//static_assert (!noexcept (tmdesc::tuple_foreach(tmdesc::make_tuple(1, '2', "42"), throw_visitor{})), "");

static_assert (noexcept (tmdesc::transform_to<::tmdesc::tuple>(tmdesc::make_tuple(1, '2', "42"), nothrow_visitor{})), "");
//static_assert (!noexcept (tmdesc::transform_t<tuple>_to(tmdesc::make_tuple(1, '2', "42"), throw_visitor{})), "");

// implementation test

#define STATIC_CHECK(...)           \
    static_assert(__VA_ARGS__, ""); \
    CHECK(__VA_ARGS__)

#define STATIC_NOTHROW_CHECK(...)             \
    static_assert(__VA_ARGS__, "");           \
    static_assert(noexcept(__VA_ARGS__), ""); \
    CHECK(__VA_ARGS__)

TEST_CASE("tuple initialisation and get") {
    SUBCASE("empty tuple creation") {
        constexpr tmdesc::tuple<> t;
        constexpr tmdesc::tuple<> clone = t;
        tmdesc::tuple<> assigned;
        assigned = clone;
        (void)assigned;
    }
    SUBCASE("single element tuple creation") {
        constexpr tmdesc::tuple<int> t{};
        constexpr tmdesc::tuple<int> clone = t;
        tmdesc::tuple<int> assigned;
        assigned = clone;

        SUBCASE("set and get") {
            tmdesc::get<0>(assigned) = 42;
            REQUIRE(tmdesc::get<0>(assigned) == 42);
        }
    }
    SUBCASE("multiple element tuple creation") {
        constexpr tmdesc::tuple<int, char> t{};
        constexpr tmdesc::tuple<int, char> clone = t;
        tmdesc::tuple<int, char> assigned;
        assigned = clone;

        SUBCASE("set and get") {
            tmdesc::get<0>(assigned) = 42;
            tmdesc::get<1>(assigned) = '3';
            REQUIRE(tmdesc::get<0>(assigned) == 42);
            REQUIRE(tmdesc::get<1>(assigned) == '3');
        }
    }
    SUBCASE("make_tuple") {
       static constexpr tmdesc::zstring_view str = "30";
        static constexpr auto t1          = tmdesc::make_tuple(str, 30, 42, 80);
       static constexpr auto t2          = t1;
        STATIC_NOTHROW_CHECK(tmdesc::get<0>(t2) == str);
        STATIC_NOTHROW_CHECK(tmdesc::get<1>(t2) == 30);
        STATIC_NOTHROW_CHECK(tmdesc::get<2>(t2) == 42);
        STATIC_NOTHROW_CHECK(tmdesc::get<3>(t2) == 80);
    }
}

TEST_CASE("tuple operations") {
    SUBCASE("empty tuple") {
        constexpr tmdesc::tuple<> t1;
        constexpr tmdesc::tuple<> t2;
        STATIC_NOTHROW_CHECK(t1 == t2);
        STATIC_NOTHROW_CHECK(!(t1 != t2));
        STATIC_NOTHROW_CHECK(!(t1 < t2));
        STATIC_NOTHROW_CHECK(!(t1 > t2));
        STATIC_NOTHROW_CHECK(t1 >= t2);
        STATIC_NOTHROW_CHECK(t1 <= t2);
    }
    SUBCASE("single same value tuple") {
        constexpr tmdesc::tuple<char> t1(char(42));
        constexpr tmdesc::tuple<int> t2(42);
        STATIC_NOTHROW_CHECK(t1 == t2);
        STATIC_NOTHROW_CHECK(!(t1 != t2));
        STATIC_NOTHROW_CHECK(!(t1 < t2));
        STATIC_NOTHROW_CHECK(!(t1 > t2));
        STATIC_NOTHROW_CHECK(t1 >= t2);
        STATIC_NOTHROW_CHECK(t1 <= t2);
    }
    SUBCASE("single different value tuple") {
        constexpr tmdesc::tuple<int> t1(1);
        constexpr tmdesc::tuple<char> t2(char(42));
        STATIC_NOTHROW_CHECK(!(t1 == t2));
        STATIC_NOTHROW_CHECK(t1 != t2);
        STATIC_NOTHROW_CHECK(t1 < t2);
        STATIC_NOTHROW_CHECK(!(t1 > t2));
        STATIC_NOTHROW_CHECK(!(t1 >= t2));
        STATIC_NOTHROW_CHECK(t1 <= t2);
    }
    SUBCASE("multiple same value tuple") {
        constexpr tmdesc::tuple<int, char, int> t1(int(1), char(-42), 3);
        constexpr tmdesc::tuple<char, int, int> t2(char(1), -42, 3);

        STATIC_NOTHROW_CHECK(t1 == t2);
        STATIC_NOTHROW_CHECK(!(t1 != t2));
        STATIC_NOTHROW_CHECK(!(t1 < t2));
        STATIC_NOTHROW_CHECK(!(t1 > t2));
        STATIC_NOTHROW_CHECK(t1 >= t2);
        STATIC_NOTHROW_CHECK(t1 <= t2);
    }

    SUBCASE("multiple different value tuple") {
        constexpr tmdesc::tuple<int, char, int> t1(1, char(-42), 3);
        constexpr tmdesc::tuple<char, int, int> t2(char(1), 42, 3);
        STATIC_NOTHROW_CHECK(!(t1 == t2));
        STATIC_NOTHROW_CHECK(t1 != t2);
        STATIC_NOTHROW_CHECK(t1 < t2);
        STATIC_NOTHROW_CHECK(!(t1 > t2));
        STATIC_NOTHROW_CHECK(!(t1 >= t2));
        STATIC_NOTHROW_CHECK(t1 <= t2);
    }
}

struct accumulator {
    int& res;
    template <class T> constexpr void operator()(const T& v) const noexcept { res += int(v); }
};

template <class T>
constexpr int
accumulate(const T& tuple) noexcept(noexcept(tmdesc::for_each(tuple, accumulator{std::declval<int&>()}))) {
    int res = 0;
    tmdesc::for_each(tuple, accumulator{res});
    return res;
}

TEST_CASE("tuple_foreach") {
    constexpr auto tuple = tmdesc::make_tuple(1, '2', 42);
    constexpr auto sum   = (1 + int('2') + 42);
    STATIC_NOTHROW_CHECK(accumulate(tuple) == sum);
}
struct increment_fn {
    template <class T> constexpr auto operator()(const T& v) const noexcept { return v + T(1); }
};

TEST_CASE("transform_t<tuple>_to") {
    constexpr auto tuple = tmdesc::make_tuple(1, '2', 42);
    constexpr auto r     = tmdesc::transform_to<tmdesc::tuple>(tuple, increment_fn{});
    STATIC_NOTHROW_CHECK(tmdesc::get<0>(r) == 2);
    STATIC_NOTHROW_CHECK(tmdesc::get<1>(r) == '3');
    STATIC_NOTHROW_CHECK(tmdesc::get<2>(r) == 43);
}
