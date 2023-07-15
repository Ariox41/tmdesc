//#include <doctest/doctest.h>

//#include "../test_helpers.hpp"
//#include <memory>
//#include <string>
//#include <tmdesc/containers/tuple.hpp>
//#include <tmdesc/string_view.hpp>

//static_assert(std::is_trivial<tmdesc::tuple<>>{}, "");
//static_assert(std::is_trivial<tmdesc::tuple<int>>{}, "");
//static_assert(std::is_trivial<tmdesc::tuple<int, char>>{}, "");

//static_assert(std::is_trivially_copyable<tmdesc::tuple<>>{}, "");
//static_assert(std::is_trivially_copyable<tmdesc::tuple<int>>{}, "");
//static_assert(std::is_trivially_copyable<tmdesc::tuple<int, char>>{}, "");

//static_assert(std::is_trivially_copy_constructible<tmdesc::tuple<>>{}, "");
//static_assert(std::is_trivially_copy_constructible<tmdesc::tuple<int>>{}, "");
//static_assert(std::is_trivially_copy_constructible<tmdesc::tuple<int, char>>{}, "");

//static_assert(std::is_trivially_copy_assignable<tmdesc::tuple<>>{}, "");
//static_assert(std::is_trivially_copy_assignable<tmdesc::tuple<int>>{}, "");
//static_assert(std::is_trivially_copy_assignable<tmdesc::tuple<int, char>>{}, "");

//static_assert(std::is_trivially_move_assignable<tmdesc::tuple<>>{}, "");
//static_assert(std::is_trivially_move_assignable<tmdesc::tuple<int>>{}, "");
//static_assert(std::is_trivially_move_assignable<tmdesc::tuple<int, char>>{}, "");

//static_assert(std::is_trivially_destructible<tmdesc::tuple<>>{}, "");
//static_assert(std::is_trivially_destructible<tmdesc::tuple<int>>{}, "");
//static_assert(std::is_trivially_destructible<tmdesc::tuple<int, char>>{}, "");

//static_assert(std::is_trivially_destructible<tmdesc::tuple<>>{}, "");
//static_assert(std::is_trivially_destructible<tmdesc::tuple<int>>{}, "");
//static_assert(std::is_trivially_destructible<tmdesc::tuple<int, char>>{}, "");

//static_assert(std::is_nothrow_default_constructible<tmdesc::tuple<>>{}, "");
//static_assert(std::is_nothrow_default_constructible<tmdesc::tuple<int>>{}, "");
//static_assert(std::is_nothrow_default_constructible<tmdesc::tuple<int, char>>{}, "");

//static_assert(std::is_nothrow_copy_assignable<tmdesc::tuple<>>{}, "");
//static_assert(std::is_nothrow_copy_assignable<tmdesc::tuple<int>>{}, "");
//static_assert(std::is_nothrow_copy_assignable<tmdesc::tuple<int, char>>{}, "");

//static_assert(!std::is_trivial<tmdesc::tuple<std::string>>{}, "");
//static_assert(!std::is_trivially_destructible<tmdesc::tuple<std::string>>{}, "");
//static_assert(!std::is_nothrow_copy_constructible<tmdesc::tuple<std::string>>{}, "");
//static_assert(!std::is_nothrow_copy_assignable<tmdesc::tuple<std::string>>{}, "");
//static_assert(std::is_nothrow_default_constructible<tmdesc::tuple<std::string>>{}, "");
//static_assert(std::is_nothrow_move_constructible<tmdesc::tuple<std::string>>{}, "");
//static_assert(std::is_nothrow_move_assignable<tmdesc::tuple<std::string>>{}, "");
//static_assert(std::is_copy_constructible<tmdesc::tuple<std::string>>{}, "");
//static_assert(std::is_copy_assignable<tmdesc::tuple<std::string>>{}, "");
//static_assert(!std::is_nothrow_copy_constructible<tmdesc::tuple<std::string>>{}, "");
//static_assert(!std::is_nothrow_copy_assignable<tmdesc::tuple<std::string>>{}, "");

//using unique_ptr_pair_tuple = tmdesc::tuple<std::unique_ptr<std::string>, std::unique_ptr<int>>;

//static_assert(!std::is_trivial<unique_ptr_pair_tuple>{}, "");
//static_assert(!std::is_trivially_destructible<unique_ptr_pair_tuple>{}, "");
//static_assert(!std::is_nothrow_copy_constructible<unique_ptr_pair_tuple>{}, "");
//static_assert(!std::is_nothrow_copy_assignable<unique_ptr_pair_tuple>{}, "");
//static_assert(std::is_nothrow_default_constructible<unique_ptr_pair_tuple>{}, "");
//static_assert(std::is_nothrow_move_constructible<unique_ptr_pair_tuple>{}, "");
//static_assert(std::is_nothrow_move_assignable<unique_ptr_pair_tuple>{}, "");
//static_assert(!std::is_copy_constructible<unique_ptr_pair_tuple>{}, "");
//static_assert(!std::is_copy_assignable<unique_ptr_pair_tuple>{}, "");

//TEST_CASE("tuple initialisation and get") {
//    SUBCASE("empty tuple") {
//        WHEN("creation") {
//            constexpr tmdesc::tuple<> t;
//            constexpr tmdesc::tuple<> cloned = t;
//            constexpr tmdesc::tuple<> moved  = std::move(cloned);
//            static_assert(tmdesc::size(t) == 0, "");
//            AND_WHEN("assignment") {
//                tmdesc::tuple<> assigned;
//                assigned = moved;
//                tmdesc::tuple<> move_assigned;
//                move_assigned = std::move(assigned);
//                (void)move_assigned;
//            }
//        }
//    }
//    SUBCASE("single element tuple") {
//        WHEN("creation") {
//            constexpr tmdesc::tuple<int> t{42};
//            constexpr tmdesc::tuple<int> cloned = t;
//            constexpr tmdesc::tuple<int> moved  = std::move(cloned);
//            static_assert(tmdesc::size(t) == 1, "");

//            AND_WHEN("assignment") {
//                tmdesc::tuple<int> assigned;
//                assigned = moved;
//                tmdesc::tuple<int> move_assigned;
//                move_assigned = std::move(assigned);
//                (void)move_assigned;

//                AND_WHEN("set and get") {
//                    tmdesc::at_c<0>(assigned) = 24;
//                    REQUIRE(tmdesc::at_c<0>(assigned) == 24);
//                    REQUIRE(tmdesc::at_c<0>(t) == 42);
//                }
//                AND_WHEN("assignment to a tuple of reference") {
//                    tmdesc::tuple<const int&> lvalue_ref_tuple = move_assigned;
//                    tmdesc::tuple<int&&> rvalue_ref_tuple      = std::move(move_assigned);

//                    REQUIRE(tmdesc::at_c<0>(lvalue_ref_tuple) == 42);
//                    REQUIRE(tmdesc::at_c<0>(rvalue_ref_tuple) == 42);
//                    tmdesc::at_c<0>(move_assigned) = 11;
//                    REQUIRE(tmdesc::at_c<0>(lvalue_ref_tuple) == 11);
//                    REQUIRE(tmdesc::at_c<0>(rvalue_ref_tuple) == 11);
//                }
//            }
//        }
//    }
//    SUBCASE("multiple element tuple creation") {
//        constexpr tmdesc::tuple<int, char> t{};
//        constexpr tmdesc::tuple<int, char> clone = t;
//        tmdesc::tuple<int, char> assigned;
//        assigned = clone;

//        SUBCASE("set and get") {
//            tmdesc::at_c<0>(assigned) = 42;
//            tmdesc::at_c<1>(assigned) = '3';
//            REQUIRE(tmdesc::at_c<0>(assigned) == 42);
//            REQUIRE(tmdesc::at_c<1>(assigned) == '3');
//        }
//    }
//    SUBCASE("make_tuple") {
//        static constexpr tmdesc::zstring_view str = "30";
//        static constexpr auto t1                  = tmdesc::make_tuple(str, 30, 42, 80);
//        static constexpr auto t2                  = t1;
//        STATIC_NOTHROW_CHECK(tmdesc::at_c<0>(t2) == str);
//        STATIC_NOTHROW_CHECK(tmdesc::at_c<1>(t2) == 30);
//        STATIC_NOTHROW_CHECK(tmdesc::at_c<2>(t2) == 42);
//        STATIC_NOTHROW_CHECK(tmdesc::at_c<3>(t2) == 80);
//    }
//}

////TEST_CASE("tuple operations") {
////    SUBCASE("empty tuple") {
////        constexpr tmdesc::tuple<> t1;
////        constexpr tmdesc::tuple<> t2;
////        STATIC_NOTHROW_CHECK(t1 == t2);
////        STATIC_NOTHROW_CHECK(!(t1 != t2));
////        STATIC_NOTHROW_CHECK(!(t1 < t2));
////        STATIC_NOTHROW_CHECK(!(t1 > t2));
////        STATIC_NOTHROW_CHECK(t1 >= t2);
////        STATIC_NOTHROW_CHECK(t1 <= t2);
////    }
////    SUBCASE("single same value tuple") {
////        constexpr tmdesc::tuple<char> t1(char(42));
////        constexpr tmdesc::tuple<int> t2(42);
////        STATIC_NOTHROW_CHECK(t1 == t2);
////        STATIC_NOTHROW_CHECK(!(t1 != t2));
////        STATIC_NOTHROW_CHECK(!(t1 < t2));
////        STATIC_NOTHROW_CHECK(!(t1 > t2));
////        STATIC_NOTHROW_CHECK(t1 >= t2);
////        STATIC_NOTHROW_CHECK(t1 <= t2);
////    }
////    SUBCASE("single different value tuple") {
////        constexpr tmdesc::tuple<int> t1(1);
////        constexpr tmdesc::tuple<char> t2(char(42));
////        STATIC_NOTHROW_CHECK(!(t1 == t2));
////        STATIC_NOTHROW_CHECK(t1 != t2);
////        STATIC_NOTHROW_CHECK(t1 < t2);
////        STATIC_NOTHROW_CHECK(!(t1 > t2));
////        STATIC_NOTHROW_CHECK(!(t1 >= t2));
////        STATIC_NOTHROW_CHECK(t1 <= t2);
////    }
////    SUBCASE("multiple same value tuple") {
////        constexpr tmdesc::tuple<int, char, int> t1(int(1), char(-42), 3);
////        constexpr tmdesc::tuple<char, int, int> t2(char(1), -42, 3);

////        STATIC_NOTHROW_CHECK(t1 == t2);
////        STATIC_NOTHROW_CHECK(!(t1 != t2));
////        STATIC_NOTHROW_CHECK(!(t1 < t2));
////        STATIC_NOTHROW_CHECK(!(t1 > t2));
////        STATIC_NOTHROW_CHECK(t1 >= t2);
////        STATIC_NOTHROW_CHECK(t1 <= t2);
////    }

////    SUBCASE("multiple different value tuple") {
////        constexpr tmdesc::tuple<int, char, int> t1(1, char(-42), 3);
////        constexpr tmdesc::tuple<char, int, int> t2(char(1), 42, 3);
////        STATIC_NOTHROW_CHECK(!(t1 == t2));
////        STATIC_NOTHROW_CHECK(t1 != t2);
////        STATIC_NOTHROW_CHECK(t1 < t2);
////        STATIC_NOTHROW_CHECK(!(t1 > t2));
////        STATIC_NOTHROW_CHECK(!(t1 >= t2));
////        STATIC_NOTHROW_CHECK(t1 <= t2);
////    }
////}

//// struct accumulator {
////     int& res;
////    template <class T> constexpr void operator()(const T& v) const noexcept { res += int(v); }
////};

//// template <class T>
//// constexpr int
////accumulate(const T& tuple) noexcept(noexcept(tmdesc::for_each(tuple, accumulator{std::declval<int&>()}))) {
////    int res = 0;
////    tmdesc::for_each(tuple, accumulator{res});
////    return res;
////}

////TEST_CASE("tuple_foreach") {
////    constexpr auto tuple = tmdesc::make_tuple(1, '2', 42);
////    constexpr auto sum   = (1 + int('2') + 42);
////    STATIC_NOTHROW_CHECK(accumulate(tuple) == sum);
////}
//// struct increment_fn {
////    template <class T> constexpr auto operator()(const T& v) const noexcept { return v + T(1); }
////};

////TEST_CASE("transform_t<tuple>_to") {
////    constexpr auto tuple = tmdesc::make_tuple(1, '2', 42);
////    constexpr auto r     = tmdesc::transform_to<tmdesc::tuple>(tuple, increment_fn{});
////    STATIC_NOTHROW_CHECK(tmdesc::at_c<0>(r) == 2);
////    STATIC_NOTHROW_CHECK(tmdesc::at_c<1>(r) == '3');
////    STATIC_NOTHROW_CHECK(tmdesc::at_c<2>(r) == 43);
////}
