#include "doctest.h"

#include <tmdesc/string_view.hpp>

#define STATIC_CHECK(...) \
    static_assert (__VA_ARGS__, ""); \
    CHECK(__VA_ARGS__)

static_assert(std::is_nothrow_default_constructible<tmdesc::string_view>{}, "");
static_assert(std::is_nothrow_copy_constructible<tmdesc::string_view>{}, "");
static_assert(std::is_nothrow_copy_assignable<tmdesc::string_view>{}, "");
static_assert(std::is_nothrow_constructible<tmdesc::string_view, const char*, std::size_t>{}, "");
static_assert(std::is_nothrow_constructible<tmdesc::string_view, const std::string&>{}, "");
static_assert(std::is_nothrow_constructible<tmdesc::string_view, std::string&>{}, "");
static_assert(std::is_nothrow_constructible<tmdesc::string_view, std::string&&>{}, "");
static_assert(std::is_nothrow_constructible<tmdesc::string_view, const char*>{}, "");

static_assert(std::is_nothrow_default_constructible<tmdesc::zstring_view>{}, "");
static_assert(std::is_nothrow_copy_constructible<tmdesc::zstring_view>{}, "");
static_assert(std::is_nothrow_copy_assignable<tmdesc::zstring_view>{}, "");
static_assert(std::is_nothrow_constructible<tmdesc::zstring_view, const char*>{}, "");
static_assert(std::is_nothrow_constructible<tmdesc::zstring_view, const std::string&>{}, "");
static_assert(std::is_nothrow_constructible<tmdesc::zstring_view, std::string&>{}, "");
static_assert(std::is_nothrow_constructible<tmdesc::zstring_view, std::string&&>{}, "");
static_assert(!std::is_constructible<tmdesc::zstring_view, const char*, std::size_t>{}, "");
static_assert(!std::is_constructible<tmdesc::zstring_view, tmdesc::string_view>{}, "");

namespace doctest {
template <> struct StringMaker<tmdesc::string_view> {
    static String convert(tmdesc::string_view str) { return String(str.data(), unsigned(str.size())); }
};
} // namespace doctest

TEST_SUITE("string_view constructors") {
    TEST_CASE("default constuctor"){
        constexpr tmdesc::string_view str;

        STATIC_CHECK(str.size() == 0);
        STATIC_CHECK(str.empty());
        STATIC_CHECK(str.begin() == str.end());
        STATIC_CHECK(str == tmdesc::string_view{});
    }
    TEST_CASE("explicit constructor from char*"){
        constexpr tmdesc::string_view str("42123");
        STATIC_CHECK(str.size() == 5);
        STATIC_CHECK(str == tmdesc::string_view("42123"));
    }
    TEST_CASE("constructor from string_view"){
        constexpr tmdesc::string_view src_str("42123");
        constexpr tmdesc::string_view str = src_str;
        STATIC_CHECK(str.size() == 5);
        STATIC_CHECK(str == src_str);
    }

    TEST_CASE("implicit constructor from char*"){
        constexpr tmdesc::string_view str = "42123";
        STATIC_CHECK(str.size() == 5);
        STATIC_CHECK(str == "42123");
    }
    TEST_CASE("implicit constructor from std::string"){
        std::string stdString("42123");
        tmdesc::string_view str = stdString;
        CHECK(str.size() == 5);
        CHECK(str == "42123");
        CHECK(str == stdString);
    }
}
TEST_SUITE("zstring_view constructors") {
    TEST_CASE("default constuctor"){
        constexpr tmdesc::zstring_view str;

        STATIC_CHECK(str.size() == 0);
        STATIC_CHECK(str.empty());
        STATIC_CHECK(str.begin() == str.end());
        STATIC_CHECK(str == tmdesc::string_view{});
        STATIC_CHECK(str[str.size()] == '\0');
    }
    TEST_CASE("explicit constructor from char*"){
        constexpr tmdesc::zstring_view str("42123");
        STATIC_CHECK(str.size() == 5);
        STATIC_CHECK(str == tmdesc::zstring_view("42123"));
        STATIC_CHECK(str[str.size()] == '\0');
    }
    TEST_CASE("implicit constructor from char*"){
        constexpr tmdesc::zstring_view str = "42123";
        STATIC_CHECK(str.size() == 5);
        STATIC_CHECK(str == "42123");
    }
    TEST_CASE("implicit constructor from std::string"){
        std::string stdString("42123");
        tmdesc::zstring_view str = stdString;
        CHECK(str.size() == 5);
        CHECK(str == "42123");
        CHECK(str == stdString);
    }
}

TEST_CASE("string_view operator[]") {
    tmdesc::string_view lha("42123");
    auto rha = "42123";
    bool eq = true;
    for (std::size_t i = 0; i < lha.size(); ++i) {
        eq = eq && lha[i] == rha[i];
    }
    CHECK(eq);
}
TEST_CASE("string_view front and back") {
    tmdesc::string_view str = "12345";
    CHECK(str.front() == '1');
    CHECK(str.back() == '5');
}
TEST_CASE("string_view remove_prefix") {
    tmdesc::string_view str = "12345678";
    auto res = str.remove_prefix(2);
    CHECK(res == "345678");
    str = res.remove_prefix(6);
    CHECK(str == "");
}
TEST_CASE("string_view remove_suffix") {
    tmdesc::string_view str = "12345678";
    auto res = str.remove_suffix(2);
    CHECK(res == "123456");
    str = res.remove_suffix(6);
    CHECK(str == "");
}
TEST_CASE("string_view substr") {
    tmdesc::string_view str = "12345678";
    auto res = str.substr(0, 8);
    CHECK(res == str);
    str = res.substr(0, 7);
    CHECK(str == "1234567");
    res = str.substr(1, 6);
    CHECK(res == "234567");
    str = res.substr(2, 2);
    CHECK(str == "45");

    CHECK(str.substr(0, 2000) == "45");
    CHECK(str.substr(3, 0) == tmdesc::string_view{});
}
TEST_CASE("string_view starts_with") {
    tmdesc::string_view str = "12345678";
    CHECK(str.starts_with('1') == true);
    CHECK(str.starts_with('2') == false);

    CHECK(str.starts_with("123") == true);
    CHECK(str.starts_with("1") == true);
    CHECK(str.starts_with("223412312412") == false);
    CHECK(str.starts_with("123456789") == false);
}
TEST_CASE("string_view ends_with") {
    tmdesc::string_view str = "12345678";
    CHECK(str.ends_with('8') == true);
    CHECK(str.ends_with('2') == false);

    CHECK(str.ends_with("678") == true);
    CHECK(str.ends_with("8") == true);
    CHECK(str.ends_with("1234") == false);
    CHECK(str.ends_with("7") == false);
    CHECK(str.ends_with("123456789") == false);
    CHECK(str.ends_with("01234567") == false);
}

TEST_CASE("string_view to_string") {
    tmdesc::string_view str = "12345678";
    std::string stdstr = str.to_string();
    CHECK(stdstr == "12345678");
}

TEST_CASE("string_view compare") {
    constexpr  tmdesc::string_view str = "12345678";
    STATIC_CHECK(str.compare("12345678") == 0);
    STATIC_CHECK(str.compare("22345678") == -1);
    STATIC_CHECK(str.compare("02345678") == 1);
    STATIC_CHECK(str.compare("12346678") == -1);
    STATIC_CHECK(str.compare("12344678") == 1);
    STATIC_CHECK(str.compare("12345679") == -1);
    STATIC_CHECK(str.compare("12345677") == 1);
    STATIC_CHECK(str.compare("123456789") == -1);
    STATIC_CHECK(str.compare("1234567") == 1);
    STATIC_CHECK(str.compare(str) == 0);
}
TEST_CASE("string_view operators") {
    constexpr tmdesc::string_view str = "12345678";
    STATIC_CHECK(str == "12345678");
    STATIC_CHECK(str != "22345678");
    STATIC_CHECK(str != "2345678");
    STATIC_CHECK(str != "123456789");
    STATIC_CHECK(str != "012345678");
    STATIC_CHECK(str < "22345678");
    STATIC_CHECK(str>"02345678");
    STATIC_CHECK(str<"12346678");
    STATIC_CHECK(str>"12344678");
    STATIC_CHECK(str < "12345679");
    STATIC_CHECK(str > "12345677");
    STATIC_CHECK(str <"123456789");
    STATIC_CHECK(str > "1234567");

    STATIC_CHECK(str <="123456789");
    STATIC_CHECK(str >= "1234567");
    STATIC_CHECK(str <= "12345678");
    STATIC_CHECK(str >= "12345678");
    STATIC_CHECK(str <= str);
    STATIC_CHECK(str >= str);
}
