#include "test_helpers.hpp"
#include <string>
#include <tmdesc/string_view.hpp>

STATIC_CHECK(std::is_nothrow_default_constructible<tmdesc::string_view>{}, "");
STATIC_CHECK(std::is_nothrow_copy_constructible<tmdesc::string_view>{}, "");
STATIC_CHECK(std::is_nothrow_copy_assignable<tmdesc::string_view>{}, "");
STATIC_CHECK(std::is_nothrow_constructible<tmdesc::string_view, const char*, std::size_t>{}, "");
STATIC_CHECK(std::is_nothrow_constructible<tmdesc::string_view, const std::string&>{}, "");
STATIC_CHECK(std::is_nothrow_constructible<tmdesc::string_view, std::string&>{}, "");
STATIC_CHECK(std::is_nothrow_constructible<tmdesc::string_view, std::string&&>{}, "");
STATIC_CHECK(std::is_nothrow_constructible<tmdesc::string_view, const char*>{}, "");

STATIC_CHECK(std::is_nothrow_default_constructible<tmdesc::zstring_view>{}, "");
STATIC_CHECK(std::is_nothrow_copy_constructible<tmdesc::zstring_view>{}, "");
STATIC_CHECK(std::is_nothrow_copy_assignable<tmdesc::zstring_view>{}, "");
STATIC_CHECK(std::is_nothrow_constructible<tmdesc::zstring_view, const char*>{}, "");
STATIC_CHECK(std::is_nothrow_constructible<tmdesc::zstring_view, const std::string&>{}, "");
STATIC_CHECK(std::is_nothrow_constructible<tmdesc::zstring_view, std::string&>{}, "");
STATIC_CHECK(std::is_nothrow_constructible<tmdesc::zstring_view, std::string&&>{}, "");
STATIC_CHECK(!std::is_constructible<tmdesc::zstring_view, const char*, std::size_t>{}, "");
STATIC_CHECK(!std::is_constructible<tmdesc::zstring_view, tmdesc::string_view>{}, "");

TEST_SUITE("string_view") {
    TEST_SUITE("string_view constructors") {
        TEST_CASE("default constructor") {
            constexpr tmdesc::string_view str;

            STATIC_CHECK(str.size() == 0);
            STATIC_CHECK(str.empty());
            STATIC_CHECK(str.begin() == str.end());
            STATIC_CHECK(str == tmdesc::string_view{});
        }
        TEST_CASE("explicit constructor from char*") {
            constexpr tmdesc::string_view str("42123");
            STATIC_CHECK(str.size() == 5);
            STATIC_CHECK(str == tmdesc::string_view("42123"));
        }
        TEST_CASE("constructor from string_view") {
            constexpr tmdesc::string_view src_str("42123");
            constexpr tmdesc::string_view str = src_str;
            STATIC_CHECK(str.size() == 5);
            STATIC_CHECK(str == src_str);
        }

        TEST_CASE("implicit constructor from char*") {
            constexpr tmdesc::string_view str = "42123";
            STATIC_CHECK(str.size() == 5);
            STATIC_CHECK(str == "42123");
        }
        TEST_CASE("implicit constructor from std::string") {
            std::string stdString("42123");
            tmdesc::string_view str = stdString;
            CHECK(str.size() == 5);
            CHECK(str == "42123");
            CHECK(str == stdString);
        }
    }
    TEST_SUITE("zstring_view constructors") {
        TEST_CASE("default constructor") {
            constexpr tmdesc::zstring_view str;

            STATIC_CHECK(str.size() == 0);
            STATIC_CHECK(str.empty());
            STATIC_CHECK(str.begin() == str.end());
            STATIC_CHECK(str == tmdesc::string_view{});
            STATIC_CHECK(*(str.c_str() + str.size()) == '\0');
        }
        TEST_CASE("explicit constructor from char*") {
            constexpr tmdesc::zstring_view str("42123");
            STATIC_CHECK(str.size() == 5);
            STATIC_CHECK(str == tmdesc::zstring_view("42123"));
            STATIC_CHECK(*(str.c_str() + str.size()) == '\0');
        }
        TEST_CASE("implicit constructor from char*") {
            constexpr tmdesc::zstring_view str = "42123";
            STATIC_CHECK(str.size() == 5);
            STATIC_CHECK(str == "42123");
        }
        TEST_CASE("implicit constructor from std::string") {
            std::string stdString("42123");
            tmdesc::zstring_view str = stdString;
            CHECK(str.size() == 5);
            CHECK(str == "42123");
            CHECK(str == stdString);
        }
    }

    TEST_CASE("operator[]") {
        tmdesc::string_view lha("42123");
        auto rha = "42123";
        bool eq  = true;
        for (std::size_t i = 0; i < lha.size(); ++i) {
            eq = eq && lha[i] == rha[i];
        }
        CHECK(eq);
    }
    TEST_CASE("front and back") {
        constexpr tmdesc::string_view str = "12345";
        STATIC_CHECK(str.front() == '1');
        STATIC_CHECK(str.back() == '5');
    }
    TEST_CASE("remove_prefix") {
        tmdesc::string_view str = "12345678";
        str.remove_prefix(2);
        CHECK(str == "345678");
        str.remove_prefix_safe(6);
        CHECK(str == "");
        str.remove_prefix_safe(1);
        CHECK(str == "");
    }
    TEST_CASE("remove_suffix") {
        tmdesc::string_view str = "12345678";
        str.remove_suffix(2);
        CHECK(str == "123456");
        str.remove_suffix_safe(6);
        CHECK(str == "");
        str.remove_suffix_safe(1);
        CHECK(str == "");
    }
    TEST_CASE("prefix") {
        tmdesc::string_view str = "12345678";
        CHECK(str.prefix(2) == "12");
        CHECK(str.prefix(0) == "");
        CHECK(str.prefix(str.size()) == str);
        CHECK(str.prefix(str.size() + 50) == str);
    }
    TEST_CASE("suffix") {
        tmdesc::string_view str = "12345678";
        CHECK(str.suffix(2) == "78");
        CHECK(str.suffix(0) == "");
        CHECK(str.suffix(str.size()) == str);
        CHECK(str.suffix(str.size() + 50) == str);
    }
    TEST_CASE("substr") {
        tmdesc::string_view str = "12345678";
        auto res                = str.substr(0, 8);
        CHECK(res == str);
        str = res.substr(0, 7);
        CHECK(str == "1234567");
        res = str.substr(1, 6);
        CHECK(res == "234567");
        str = res.substr(2, 2);
        CHECK(str == "45");

        CHECK(str.substr(0, 2000) == "45");
        CHECK(str.substr(2, 2000) == "");
        CHECK_THROWS(str.substr(3, 4));
        CHECK_THROWS(str.substr(3, 1));
        CHECK(str.substr_safe(3, 0) == tmdesc::string_view{});
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
        std::string stdstr      = str.to_string();
        CHECK(stdstr == "12345678");
    }

    TEST_CASE("string_view compare") {
        constexpr tmdesc::string_view str = "12345678";
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
        STATIC_CHECK(str > "02345678");
        STATIC_CHECK(str < "12346678");
        STATIC_CHECK(str > "12344678");
        STATIC_CHECK(str < "12345679");
        STATIC_CHECK(str > "12345677");
        STATIC_CHECK(str < "123456789");
        STATIC_CHECK(str > "1234567");

        STATIC_CHECK(str <= "123456789");
        STATIC_CHECK(str >= "1234567");
        STATIC_CHECK(str <= "12345678");
        STATIC_CHECK(str >= "12345678");
        STATIC_CHECK(str <= str);
        STATIC_CHECK(str >= str);
    }
    TEST_CASE("string_view find") {
        constexpr tmdesc::string_view str = "123456787654321";
        STATIC_CHECK(str.find('1') == 0);
        STATIC_CHECK(str.find('1', 2) == str.size() - 1);

        STATIC_CHECK(str.find('2') == 1);
        STATIC_CHECK(str.find('2', 3) == str.size() - 2);

        STATIC_CHECK(str.find("12") == 0);
        STATIC_CHECK(str.find("21", 2) == str.size() - 2);

        STATIC_CHECK(str.find("23") == 1);
        STATIC_CHECK(str.find("32", 3) == str.size() - 3);
    }
    TEST_CASE("string_view rfind") {
        constexpr tmdesc::string_view str = "123456787654321";
        STATIC_CHECK(str.rfind('1') == str.size() - 1);
        STATIC_CHECK(str.rfind('1', 2) == 0);

        STATIC_CHECK(str.rfind('2') == str.size() - 2);
        STATIC_CHECK(str.rfind('2', 3) == 1);

        STATIC_CHECK(str.rfind("21") == str.size() - 2);
        STATIC_CHECK(str.rfind("34", 6) == 2);

        STATIC_CHECK(str.rfind("32") == str.size() - 3);
        STATIC_CHECK(str.rfind("23", 1) == 1);
    }
    TEST_CASE("string_view find_first_of") {
        constexpr tmdesc::string_view str = "123456787654321";
        STATIC_CHECK(str.find_first_of('1') == 0);
        STATIC_CHECK(str.find_first_of('1', 2) == str.size() - 1);

        STATIC_CHECK(str.find_first_of('2') == 1);
        STATIC_CHECK(str.find_first_of('2', 3) == str.size() - 2);

        STATIC_CHECK(str.find_first_of("32") == 1);
        STATIC_CHECK(str.find_first_of("21", 2) == str.size() - 2);

        STATIC_CHECK(str.find_first_of("23") == 1);
        STATIC_CHECK(str.find_first_of("32", 3) == str.size() - 3);
    }
    TEST_CASE("string_view find_last_of") {
        constexpr tmdesc::string_view str = "123456787654321";
        STATIC_CHECK(str.find_last_of('1') == str.size() - 1);
        STATIC_CHECK(str.find_last_of('1', 2) == 0);

        STATIC_CHECK(str.find_last_of('2') == str.size() - 2);
        STATIC_CHECK(str.find_last_of('2', 3) == 1);

        STATIC_CHECK(str.find_last_of("12345") == str.size() - 1);
        STATIC_CHECK(str.find_last_of("12345", 2) == 2);

        STATIC_CHECK(str.find_last_of("2345") == str.size() - 2);
        STATIC_CHECK(str.find_last_of("2345", 8) == 4);
    }
}
