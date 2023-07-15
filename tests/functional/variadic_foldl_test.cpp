#include "../test_helpers.hpp"
#include <tmdesc/functional/variadic_fold_left.hpp>

struct sub_t {
    constexpr int operator()(int lha) const noexcept { return lha; }
    constexpr int operator()(int lha, int rha) const noexcept { return lha - rha; }
};
inline constexpr sub_t sub{};

STATIC_CHECK(!tmdesc::is_invocable_v<tmdesc::variadic_fold_left_t, sub_t>);

STATIC_CHECK(tmdesc::is_nothrow_invocable_v<tmdesc::variadic_fold_left_t, sub_t, int>);
STATIC_CHECK(std::is_same_v<int, tmdesc::invoke_result_t<tmdesc::variadic_fold_left_t, sub_t, int>>);
STATIC_CHECK(tmdesc::variadic_fold_left(sub, 42) == 42);

STATIC_CHECK(tmdesc::is_nothrow_invocable_v<tmdesc::variadic_fold_left_t, sub_t, int, int>);
STATIC_CHECK(std::is_same_v<int, tmdesc::invoke_result_t<tmdesc::variadic_fold_left_t, sub_t, int, int>>);
STATIC_CHECK(tmdesc::variadic_fold_left(sub, 42, 2) == 42 - 2);

STATIC_CHECK(tmdesc::is_nothrow_invocable_v<tmdesc::variadic_fold_left_t, sub_t, int, int, int>);
STATIC_CHECK(std::is_same_v<int, tmdesc::invoke_result_t<tmdesc::variadic_fold_left_t, sub_t, int, int, int>>);
STATIC_CHECK(tmdesc::variadic_fold_left(sub, 42, 3, 2, 1) == 42 - 3 - 2 - 1);

STATIC_CHECK(tmdesc::is_nothrow_invocable_v<tmdesc::variadic_fold_left_t, sub_t, int, int, int, int>);
STATIC_CHECK(std::is_same_v<int, tmdesc::invoke_result_t<tmdesc::variadic_fold_left_t, sub_t, int, int, int, int>>);
STATIC_CHECK(tmdesc::variadic_fold_left(sub, 42, 4, 3, 2, 1) == 42 - 4 - 3 - 2 - 1);

STATIC_CHECK(tmdesc::is_nothrow_invocable_v<tmdesc::variadic_fold_left_t, sub_t, int, int, int, int, int>);
STATIC_CHECK(
    std::is_same_v<int, tmdesc::invoke_result_t<tmdesc::variadic_fold_left_t, sub_t, int, int, int, int, int>>);
STATIC_CHECK(tmdesc::variadic_fold_left(sub, 42, 5, 4, 3, 2, 1) == 42 - 5 - 4 - 3 - 2 - 1);

STATIC_CHECK(tmdesc::is_nothrow_invocable_v<tmdesc::variadic_fold_left_t, sub_t, int, int, int, int, int, int>);
STATIC_CHECK(
    std::is_same_v<int, tmdesc::invoke_result_t<tmdesc::variadic_fold_left_t, sub_t, int, int, int, int, int, int>>);
STATIC_CHECK(tmdesc::variadic_fold_left(sub, 42, 6, 5, 4, 3, 2, 1) == 42 - 6 - 5 - 4 - 3 - 2 - 1);

STATIC_CHECK(tmdesc::variadic_fold_left(sub, 1, 2, 3, 4, 5, 6, 7, 8) == 1 - 2 - 3 - 4 - 5 - 6 - 7 - 8);
STATIC_CHECK(tmdesc::variadic_fold_left(sub, 1, 2, 3, 4, 5, 6, 7, 8, 9) == 1 - 2 - 3 - 4 - 5 - 6 - 7 - 8 - 9);
STATIC_CHECK(tmdesc::variadic_fold_left(sub, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10) == 1 - 2 - 3 - 4 - 5 - 6 - 7 - 8 - 9 - 10);
STATIC_CHECK(tmdesc::variadic_fold_left(sub, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11) ==
             1 - 2 - 3 - 4 - 5 - 6 - 7 - 8 - 9 - 10 - 11);
STATIC_CHECK(tmdesc::variadic_fold_left(sub, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12) ==
             1 - 2 - 3 - 4 - 5 - 6 - 7 - 8 - 9 - 10 - 11 - 12);
STATIC_CHECK(tmdesc::variadic_fold_left(sub, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13) ==
             1 - 2 - 3 - 4 - 5 - 6 - 7 - 8 - 9 - 10 - 11 - 12 - 13);
STATIC_CHECK(tmdesc::variadic_fold_left(sub, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14) ==
             1 - 2 - 3 - 4 - 5 - 6 - 7 - 8 - 9 - 10 - 11 - 12 - 13 - 14);
STATIC_CHECK(tmdesc::variadic_fold_left(sub, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15) ==
             1 - 2 - 3 - 4 - 5 - 6 - 7 - 8 - 9 - 10 - 11 - 12 - 13 - 14 - 15);
