#include <doctest/doctest.h>

#define STATIC_CHECK(...) static_assert((__VA_ARGS__), "");
#define STATIC_CHECK_NOTHROW(...) static_assert(noexept(__VA_ARGS__), "");
#define STATIC_CHECK_VALUE_AND_NOTHROW(...) \
    STATIC_CHECK(__VA_ARGS__);              \
    STATIC_CHECK_NOTHROW(__VA_ARGS__)

namespace tmdesc {
class string_view;
class zstring_view;
} // namespace tmdesc
namespace doctest {
template <> struct StringMaker<tmdesc::string_view> {
    template <class T> static String convert(T str) { return String(str.data(), unsigned(str.size())); }
};
template <> struct StringMaker<tmdesc::zstring_view> {
    template <class T> static String convert(T str) { return String(str.data(), unsigned(str.size())); }
};

} // namespace doctest
