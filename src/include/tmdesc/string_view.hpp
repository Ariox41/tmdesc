// Copyright Victor Smirnov 2021
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
//
// The documentation can be found at the library's page:
// https://github.com/Ariox41/tmdesc

#pragma once
#include <iosfwd> //< std::string foward declaration
#include <string_view>
#include <type_traits>

namespace tmdesc {

/// Minor extension of std::string_view
/// @see class zstring_view
class string_view : public std::string_view {
public:
    using std::string_view::string_view;
    using std::string_view::operator=;

    template <typename T, std::enable_if_t<std::is_base_of_v<std::string, T>, bool> = true>
    string_view(const T& src) noexcept
      : string_view{src.data(), src.size()} {}

    /// @return prefix with size of min(n, this->size())
    constexpr string_view prefix(std::size_t n) const noexcept {
        if (n > this->size())
            n = this->size();
        return string_view{this->data(), n};
    }

    /// @return suffix with size of min(n, this->size())
    constexpr string_view suffix(std::size_t n) const noexcept {
        if (n > this->size())
            n = this->size();
        return string_view{this->data() + this->size() - n, n};
    }

    /// @return substring {pos, pos + count}
    /// @note noexcept, unlike substr.
    /// if(pos > size()) pos = size();
    /// if((pos + count) > size()) count = size() - pos;
    constexpr string_view substr_safe(std::size_t pos, std::size_t count = npos) const noexcept {
        if (pos > this->size())
            pos = this->size();
        if (pos + count > this->size())
            count = this->size() - pos;
        return string_view(this->data() + pos, count);
    }

    /// starts_with implementation for c++ 17
    constexpr bool starts_with(char ch) const noexcept { return this->size() > 0 && this->front() == ch; }

    /// starts_with implementation for c++ 17
    constexpr bool starts_with(string_view s) const noexcept { return this->prefix(s.size()) == s; }

    /// ends_with implementation for c++ 17
    constexpr bool ends_with(char ch) const noexcept { return this->size() > 0 && this->back() == ch; }

    /// ends_with implementation for c++ 17
    constexpr bool ends_with(string_view s) const noexcept { return this->suffix(s.size()) == s; }

    /// Remove prefix with size of min(n, this->size())
    constexpr void remove_prefix_safe(std::size_t n) noexcept {
        std::string_view::remove_prefix(n > this->size() ? this->size() : n);
    }

    /// Remove suffix with size of min(n, this->size())
    constexpr void remove_suffix_safe(std::size_t n) noexcept {
        std::string_view::remove_suffix(n > this->size() ? this->size() : n);
    }

    /// convert to std::string or other compability type
    template <class String = std::string>
    constexpr String to_string(std::void_t<String>* = 0) const
        noexcept(noexcept(String{std::declval<const char*>(), size_t{}})) {
        return {data(), size()};
    }
};

/// Null-terminated string_view.
/// @details Constucts from const char* and std::string only, default constructor equals zstring_view("").
class zstring_view : public string_view {
public:
    /// Default constructor equals to zstring_view("")
    /// @post
    /// size() == 0
    /// *data() == '\0'
    /// *c_str() == '\0'
    constexpr zstring_view() noexcept
      : zstring_view("") {}

    /// Construct from null-terminated string.
    /// @post
    /// c_str() == cstr? cstd: ""
    /// data() == c_str()
    /// size() == (cstr != nullptr? std::strlen(cstr): 0)
    /// @note implicit constructor, like std::string_view
    constexpr zstring_view(const char* cstr) noexcept
      : string_view(cstr ? cstr : "") {}

    /// Construct from std::string
    /// @post
    /// data() == src.c_str()
    /// size() == src.size()
    /// c_str() == src.c_str()
    /// (*this)[size()] == '\0'
    template <typename T, std::enable_if_t<std::is_base_of_v<std::string, T>, bool> = true>
    zstring_view(const T& src) noexcept
      : string_view{src.c_str(), src.size()} {}

    zstring_view(std::nullptr_t) = delete;
    zstring_view(string_view)    = delete;

    constexpr const char* c_str() const noexcept { return data(); }
};

} // namespace tmdesc
