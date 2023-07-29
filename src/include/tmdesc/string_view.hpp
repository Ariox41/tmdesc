// Copyright Victor Smirnov 2021
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
//
// The documentation can be found at the library's page:
// https://github.com/Ariox41/tmdesc

#pragma once
#include <iosfwd> //< std::string foward declaration
#include <tmdesc/config.hpp>
#include <type_traits>

#ifdef TMDESC_CONFIG_ENABLE_STD_STRING_VIEW
#include <string_view>
namespace tmdesc {
using basic_string_view = std::string_view;
}
#else
#include <cassert>
#include <exception>
namespace tmdesc {
class basic_string_view;

constexpr bool operator==(basic_string_view lha, basic_string_view rha) noexcept;
constexpr bool operator!=(basic_string_view lha, basic_string_view rha) noexcept;

/// constexpr basic_string_view for c++ 14.
/// The interface is based on c++17 std::string_view and boost::string_view
/// \see class zstring_view
class basic_string_view {
    const char* data_;
    std::size_t size_;

public:
    static constexpr std::size_t npos = -1;

    using value_type      = const char;
    using const_pointer   = const value_type*;
    using pointer         = const_pointer;
    using const_reference = value_type&;
    using reference       = const_reference;
    using const_iterator  = const_pointer;
    using iterator        = const_iterator;
    using size_type       = std::size_t;
    using difference_type = std::ptrdiff_t;

    /// @note stdexcept is too big a dependency. This exception is intended for minimal logical compatibility with std::string, but the type of exception is different
    class out_of_range : public std::exception {
        const char* what() const noexcept override { return "tmdesc::string_view out_of_range"; }
    };

    /// Constructs an empty basic_string_view.
    /// \post
    /// size() == 0
    /// begin() == end()
    /// data() == undefined
    /// \see zstring_view::zstring_view()
    constexpr basic_string_view() noexcept
      : basic_string_view(nullptr, 0) {}

    /// Construct from pointer and size
    /// \post
    /// size() == size
    /// data() == str
    constexpr basic_string_view(const char* str, const std::size_t size) noexcept
      : data_(str)
      , size_(size) {}

    /// Construct from null-terminated string.
    /// \post
    /// size() == (cstr == nullptr? std::strlen(cstr): 0)
    /// \note implicit constructor, like std::string_view
    constexpr basic_string_view(const char* cstr) noexcept
      : basic_string_view(cstr, cstringsize(cstr)) {}

    template <typename T, std::enable_if_t<std::is_base_of<std::string, T>::value, bool> = true>
    constexpr basic_string_view(const T& src) noexcept
      : basic_string_view{src.data(), src.size()} {}

    constexpr basic_string_view(const basic_string_view&) noexcept   = default;
    constexpr basic_string_view& operator=(const basic_string_view&) = default;

    constexpr basic_string_view(std::nullptr_t) = delete;

    constexpr const char* data() const noexcept { return data_; }
    constexpr std::size_t size() const noexcept { return size_; }
    constexpr bool empty() const noexcept { return size() == 0; }

    constexpr const char* cbegin() const noexcept { return data(); }
    constexpr const char* cend() const noexcept { return data() + size(); }
    constexpr const char* begin() const noexcept { return cbegin(); }
    constexpr const char* end() const noexcept { return cend(); }

    constexpr char operator[](std::size_t i) const noexcept { return data()[i]; }

    constexpr char front() const noexcept { return (*this)[0]; }
    constexpr char back() const noexcept { return (*this)[size() - 1]; }

    /// \return substring {pos, pos + count}
    /// \note noexcept, unlike std::string_view.
    /// if(pos > size()) pos = size();
    /// if((pos + count) > size()) count = size() - pos;
    constexpr basic_string_view substr(std::size_t pos, std::size_t count = npos) const {
        if (pos > size_)
            throw out_of_range{};
        if (pos + count > size_)
            count = size_ - pos;
        return basic_string_view(data_ + pos, count);
    }

    /// Remove prefix with size of min(n, this->size())
    constexpr void remove_prefix(std::size_t n) noexcept {
        assert(n <= size_);
        data_ += n;
        size_ -= n;
    }

    /// Remove suffix with size of min(n, this->size())
    constexpr void remove_suffix(std::size_t n) noexcept {
        assert(n <= size_);
        size_ -= n;
    }

    constexpr std::size_t find_first_of(char ch, std::size_t pos = 0) const noexcept {
        for (std::size_t i = pos; i < size(); ++i) {
            if ((*this)[i] == ch)
                return i;
        }
        return npos;
    }

    constexpr std::size_t find_first_of(basic_string_view symbols, std::size_t pos = 0) const noexcept {
        for (std::size_t i = pos; i < size(); ++i) {
            if (symbols.find_first_of((*this)[i]) != npos) {
                return i;
            }
        }
        return npos;
    }

    constexpr std::size_t find_last_of(char ch, std::size_t pos = npos) const noexcept {
        if (pos >= size())
            pos = size() - 1;
        for (std::size_t i = pos; i < size(); --i) {
            if ((*this)[i] == ch)
                return i;
        }
        return npos;
    }

    constexpr std::size_t find_last_of(basic_string_view symbols, std::size_t pos = npos) const noexcept {
        if (pos > size() - 1)
            pos = size() - 1;
        for (std::size_t i = pos; i < size(); --i) {
            if (symbols.find_first_of((*this)[i]) != npos)
                return i;
        }
        return npos;
    }

    constexpr basic_string_view prefix(std::size_t n) const noexcept {
        if (n > this->size())
            n = this->size();
        return basic_string_view{this->data(), n};
    }

    constexpr basic_string_view suffix(std::size_t n) const noexcept {
        if (n > this->size())
            n = this->size();
        return basic_string_view{this->data() + this->size() - n, n};
    }

    constexpr bool starts_with(basic_string_view s) const noexcept { return this->prefix(s.size()) == s; }

    constexpr bool ends_with(basic_string_view s) const noexcept { return this->suffix(s.size()) == s; }

    constexpr std::size_t find(char ch, std::size_t pos = 0) const noexcept { return find_first_of(ch, pos); }
    constexpr std::size_t find(basic_string_view pattern, std::size_t pos = 0) const noexcept {
        for (std::size_t i = pos; i < size(); ++i) {
            if (suffix(size() - i).starts_with(pattern))
                return i;
        }
        return npos;
    }
    constexpr std::size_t rfind(char ch, std::size_t pos = npos) const noexcept { return find_last_of(ch, pos); }
    constexpr std::size_t rfind(basic_string_view pattern, std::size_t pos = npos) const noexcept {
        if (pos > size() - 1)
            pos = size() - 1;
        for (std::size_t i = pos; i < size(); --i) {
            if (suffix(size() - i).starts_with(pattern))
                return i;
        }
        return npos;
    }

    constexpr int compare(basic_string_view s) const noexcept {
        auto minSize = this->size() < s.size() ? this->size() : s.size();
        for (std::size_t i = 0; i < minSize; ++i) {
            if ((*this)[i] != s[i]) {
                return (*this)[i] < s[i] ? -1 : 1;
            }
        }
        if (this->size() == s.size())
            return 0;
        return this->size() < s.size() ? -1 : 1;
    }

private:
    static constexpr std::size_t cstringsize(const char* str) noexcept {
        std::size_t size = 0;
        if (str) {
            for (; str[size] != '\0'; ++size) {}
        }
        return size;
    }
};

constexpr bool operator==(basic_string_view lha, basic_string_view rha) noexcept {
    if (lha.size() != rha.size())
        return false;
    for (std::size_t i = 0; i < lha.size(); ++i) {
        if (lha[i] != rha[i]) {
            return false;
        }
    }
    return true;
}
constexpr bool operator!=(basic_string_view lha, basic_string_view rha) noexcept { return !(lha == rha); }

constexpr bool operator<(basic_string_view lha, basic_string_view rha) noexcept { return lha.compare(rha) == -1; }
constexpr bool operator<=(basic_string_view lha, basic_string_view rha) noexcept { return lha.compare(rha) != 1; }
constexpr bool operator>(basic_string_view lha, basic_string_view rha) noexcept { return lha.compare(rha) == 1; }
constexpr bool operator>=(basic_string_view lha, basic_string_view rha) noexcept { return lha.compare(rha) != -1; }
} // namespace tmdesc
#endif

namespace tmdesc {

/// Minor extension of std::string_view
/// @see class zstring_view
class string_view : public basic_string_view {
public:
    using basic_string_view::basic_string_view;
    using basic_string_view::operator=;

    string_view(const string_view&)            = default;
    string_view& operator=(const string_view&) = default;

    template <typename T, std::enable_if_t<std::is_base_of<std::string, T>::value, bool> = true>
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

    constexpr string_view substr(std::size_t pos, std::size_t count = npos) const {
        auto s = basic_string_view::substr(pos, count);
        return string_view(s.data(), s.size());
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
        basic_string_view::remove_prefix(n > this->size() ? this->size() : n);
    }

    /// Remove suffix with size of min(n, this->size())
    constexpr void remove_suffix_safe(std::size_t n) noexcept {
        basic_string_view::remove_suffix(n > this->size() ? this->size() : n);
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

    zstring_view(const zstring_view&)            = default;
    zstring_view& operator=(const zstring_view&) = default;

    /// Construct from std::string
    /// @post
    /// data() == src.c_str()
    /// size() == src.size()
    /// c_str() == src.c_str()
    /// (*this)[size()] == '\0'
    template <typename T, std::enable_if_t<std::is_base_of<std::string, T>::value, bool> = true>
    zstring_view(const T& src) noexcept
      : string_view{src.c_str(), src.size()} {}

    zstring_view(std::nullptr_t) = delete;
    zstring_view(string_view)    = delete;

    constexpr const char* c_str() const noexcept { return data(); }
};

} // namespace tmdesc
