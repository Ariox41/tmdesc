// Copyright Victor Smirnov 2021
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
//
// The documentation can be found at the library's page:
// https://github.com/Ariox41/tmdesc

#pragma once

#include <cstdint>
#include <string>

namespace tmdesc {

class string_view;

constexpr bool operator==(string_view lha, string_view rha) noexcept;
constexpr bool operator!=(string_view lha, string_view rha) noexcept;

/// constexpr string_view for c++ 14.
/// The interface is based on c++17 std::string_view and boost::string_view
/// \see class zstring_view
class string_view {
    const char* m_str;
    std::size_t m_size;

public:
    static constexpr std::size_t npos = std::string::npos;

    using value_type      = const char;
    using const_pointer   = const value_type*;
    using pointer         = const_pointer;
    using const_reference = value_type&;
    using reference       = const_reference;
    using const_iterator  = const_pointer;
    using iterator        = const_iterator;
    using size_type       = std::size_t;
    using difference_type = std::ptrdiff_t;

    /// Constructs an empty string_view.
    /// \post
    /// size() == 0
    /// begin() == end()
    /// data() == undefined
    /// \see zstring_view::zstring_view()
    constexpr string_view() noexcept
      : string_view(nullptr, 0) {}

    /// Construct from pointer and size
    /// \post
    /// size() == size
    /// data() == str
    constexpr string_view(const char* str, const std::size_t size) noexcept
      : m_str(str)
      , m_size(size) {}

    /// Construct from null-terminated string.
    /// \post
    /// size() == (cstr == nullptr? std::strlen(cstr): 0)
    /// \note implicit constructor, like std::string_view
    constexpr string_view(const char* cstr) noexcept
      : string_view(cstr, cstringsize(cstr)) {}

    constexpr string_view(std::nullptr_t) = delete;

    /// Construct from std::string.
    /// \post
    /// size() == src.size()
    /// data() == src.data()
    /// data()[size()] is undefined behavior
    /// \note rvalue std::string is supported
    /// \code
    /// std::string getString();
    /// if(tmdesc::string_view(getString()).ends_with("suffix"){
    ///     /*...*/
    /// }
    /// \see zstring_view::string_view(const std::basic_string<char, Ts...>&)
    template <class... Ts>
    string_view(const std::basic_string<char, Ts...>& str) noexcept
      : string_view(str.data(), str.size()) {}

    constexpr string_view(const string_view&) noexcept = default;
    constexpr string_view(string_view&&) noexcept = default;
    constexpr string_view& operator=(const string_view&) noexcept = default;
    constexpr string_view& operator=(string_view&&) noexcept = default;

    constexpr const char* data() const noexcept { return m_str; }
    constexpr std::size_t size() const noexcept { return m_size; }
    constexpr bool empty() const noexcept { return size() == 0; }

    constexpr const char* cbegin() const noexcept { return data(); }
    constexpr const char* cend() const noexcept { return data() + size(); }
    constexpr const char* begin() const noexcept { return cbegin(); }
    constexpr const char* end() const noexcept { return cend(); }

    constexpr char operator[](std::size_t i) const noexcept { return data()[i]; }

    constexpr char front() const noexcept { return (*this)[0]; }
    constexpr char back() const noexcept { return (*this)[size() - 1]; }

    /// Remove prefix with size of min(n, this->size())
    constexpr string_view remove_prefix(std::size_t n) const noexcept {
        std::size_t realN = n > size() ? size() : n;
        return string_view{data() + realN, size() - realN};
    }

    /// Remove suffix with size of min(n, this->size())
    constexpr string_view remove_suffix(std::size_t n) const noexcept {
        std::size_t realN = n > size() ? size() : n;
        return string_view{begin(), size() - realN};
    }
    /// return substring {pos, pos + count}
    /// \note noexcept, unlike std::string_view.
    /// if(pos > size()) pos = size();
    /// if((pos + count) > size()) count = size() - pos;
    constexpr string_view substr(std::size_t pos, std::size_t count = npos) const noexcept {
        if (pos > size())
            pos = size();
        if ((pos + count) > size())
            count = size() - pos;
        return string_view(data() + pos, count);
    }
    constexpr bool starts_with(char ch) const noexcept { return size() > 0 && front() == ch; }
    constexpr bool starts_with(string_view s) const noexcept { return substr(0, s.size()) == s; }
    constexpr bool ends_with(char ch) const noexcept { return size() > 0 && back() == ch; }
    constexpr bool ends_with(string_view s) const noexcept {
        return substr(size() - s.size(), s.size()) == s;
    }

    constexpr std::size_t find_first_of(char ch) const noexcept {
        for (std::size_t i = 0; i < size(); ++i) {
            if ((*this)[i] == ch)
                return i;
        }
        return npos;
    }
    constexpr std::size_t find_first_of(string_view symbols) const noexcept {
        for (std::size_t i = 0; i < size(); ++i) {
            if (symbols.find_first_of((*this)[i]) != npos)
                return i;
        }
        return npos;
    }

    constexpr int compare(string_view s) const noexcept {
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

    /// \note convert to std::string.
    /// \note not compatible with std::string_view, but compatible with boost::string_view
    std::string to_string() const { return std::string(data(), size()); }

private:
    static constexpr std::size_t cstringsize(const char* str) noexcept {
        std::size_t size = 0;
        if (str) {
            for (; str[size] != '\0'; ++size) {}
        }
        return size;
    }
};

/// Null-terminated string_view.
/// Constucts from const char* and std::string only, default constuctor equals zstring_view("").
class zstring_view : public string_view {

public:
    /// Default constuctor equals to zstring_view("")
    /// \post
    /// size() == 0
    /// *data() == '\0'
    /// *c_str() == '\0'
    constexpr zstring_view() noexcept
      : zstring_view("") {}

    /// Construct from null-terminated string.
    /// \post
    /// size() == (cstr == nullptr? std::strlen(cstr): 0)
    /// \note implicit constructor, like std::string_view
    constexpr zstring_view(const char* cstr) noexcept
      : string_view(cstr) {}

    /// Construct from std::string
    /// \post
    /// size() == src.size()
    /// c_str() == src.c_str()
    /// data() == src.c_str()
    /// (*this)[size()] == '\0'
    template <class... Ts>
    zstring_view(const std::basic_string<char, Ts...>& src) noexcept
      : string_view(src.c_str(), src.size()) {}

    constexpr const char* c_str() const noexcept { return data(); }
};

constexpr bool operator==(string_view lha, string_view rha) noexcept {
    if (lha.size() != rha.size())
        return false;
    for (std::size_t i = 0; i < lha.size(); ++i) {
        if (lha[i] != rha[i]) {
            return false;
        }
    }
    return true;
}
constexpr bool operator!=(string_view lha, string_view rha) noexcept { return !(lha == rha); }

constexpr bool operator<(string_view lha, string_view rha) noexcept {
    return lha.compare(rha) == -1;
}
constexpr bool operator<=(string_view lha, string_view rha) noexcept {
    return lha.compare(rha) != 1;
}
constexpr bool operator>(string_view lha, string_view rha) noexcept {
    return lha.compare(rha) == 1;
}
constexpr bool operator>=(string_view lha, string_view rha) noexcept {
    return lha.compare(rha) != -1;
}

} // namespace tmdesc
