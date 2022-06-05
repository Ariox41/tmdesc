#include <doctest/doctest.h>

#define STATIC_CHECK(...) static_assert((__VA_ARGS__), "");
#define STATIC_CHECK_NOTHROW(...) static_assert(noexept(__VA_ARGS__), "");
#define STATIC_CHECK_VALUE_AND_NOTHROW(...) STATIC_CHECK(__VA_ARGS__); STATIC_CHECK_NOTHROW(__VA_ARGS__)

#include <tmdesc/string_view.hpp>

namespace doctest {
template <> struct StringMaker<tmdesc::string_view> {
    static String convert(tmdesc::string_view str) { return String(str.data(), unsigned(str.size())); }
};
template <> struct StringMaker<tmdesc::zstring_view> {
    static String convert(tmdesc::string_view str) { return String(str.data(), unsigned(str.size())); }
};

} // namespace doctest
