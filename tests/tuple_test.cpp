#include <doctest/doctest.h>

#include <string>
#include <tmdesc/algorithm/for_each.hpp>
#include <tmdesc/algorithm/transform.hpp>
#include <tmdesc/string_view.hpp>
#include <tmdesc/tuple/tuple.hpp>
#include <tmdesc/tuple/tuple_operators.hpp>

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

struct nothrow_visitor {
    template <class T> constexpr bool operator()(const T&) const noexcept { return false; }
};
struct throw_visitor {
    template <class T> constexpr bool operator()(const T&) const noexcept(false) {
        throw "";
        return false;
    }
};

//static_assert (!noexcept (tmdesc::repeat_n<4>(throw_visitor{})), "");

static_assert(noexcept(tmdesc::for_each(tmdesc::make_tuple(1, '2', "42"), nothrow_visitor{})), "");

static_assert(noexcept(tmdesc::transform_to<::tmdesc::tuple>(tmdesc::make_tuple(1, '2', "42"), nothrow_visitor{})), "");

// ?? msvc sometimes considers a function noexcept(true)
// ?? if noexcept(false) is in its description, but `throw` is not called in its implementation.
//static_assert (!noexcept (tmdesc::tuple_foreach(tmdesc::make_tuple(1, '2', "42"), throw_visitor{})), "");
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
    SUBCASE("empty tuple constuctors and assigment") {
        constexpr tmdesc::tuple<> t;
        constexpr tmdesc::tuple<> cloned = t;
        constexpr tmdesc::tuple<> moved  = std::move(cloned);
        tmdesc::tuple<> assigned;
        assigned = moved;
        tmdesc::tuple<> move_assigned;
        move_assigned = std::move(assigned);
        (void)move_assigned;
    }
    SUBCASE("single element tuple constuctors and assigment") {
        constexpr tmdesc::tuple<int> t{42};
        constexpr tmdesc::tuple<int> cloned = t;
        constexpr tmdesc::tuple<int> moved  = std::move(cloned);
        tmdesc::tuple<int> assigned;
        assigned = moved;
        tmdesc::tuple<int> move_assigned;
        move_assigned = std::move(assigned);
        (void)move_assigned;

        SUBCASE("set and get") {
            tmdesc::get<0>(assigned) = 24;
            REQUIRE(tmdesc::get<0>(assigned) == 24);
            REQUIRE(tmdesc::get<0>(t) == 42);
        }
        SUBCASE("to tuple of reference") {
            tmdesc::tuple<const int&> tuple_of_ref = assigned;
            REQUIRE(tmdesc::get<0>(tuple_of_ref) == 42);
            tmdesc::get<0>(assigned) = 11;
            REQUIRE(tmdesc::get<0>(tuple_of_ref) == 11);
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
        static constexpr auto t1                  = tmdesc::make_tuple(str, 30, 42, 80);
        static constexpr auto t2                  = t1;
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
