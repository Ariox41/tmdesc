// Copyright Victor Smirnov 2021-2022
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
//
// The documentation can be found at the library's page:
// https://github.com/Ariox41/tmdesc

#pragma once
#include <stdexcept>
#include <string>

namespace tmdesc {

class string_view;

constexpr bool operator==(string_view lha, string_view rha) noexcept;
constexpr bool operator!=(string_view lha, string_view rha) noexcept;

/**
 * @brief constexpr string_view for c++ 14.
 * @details The interface is based on c++ 17 std::string_view and boost::string_view
 * @see class zstring_view
 */
class string_view {
    const char* data_;
    std::size_t size_;

public:
    using value_type      = const char;
    using const_pointer   = const value_type*;
    using pointer         = const_pointer;
    using const_reference = value_type&;
    using reference       = const_reference;
    using const_iterator  = const_pointer;
    using iterator        = const_iterator;
    using size_type       = std::size_t;
    using difference_type = std::ptrdiff_t;

    static constexpr size_type npos = std::string::npos;

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
      : data_(str)
      , size_(size) {}

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
    /// \endcode
    /// \see zstring_view::string_view(const std::basic_string<char, Ts...>&)
    template <class... Ts>
    string_view(const std::basic_string<char, Ts...>& str) noexcept
      : string_view(str.data(), str.size()) {}

    constexpr string_view(const string_view&) noexcept = default;
    constexpr string_view(string_view&&) noexcept      = default;
    constexpr string_view& operator=(const string_view&) noexcept = default;
    constexpr string_view& operator=(string_view&&) noexcept = default;

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

    /**
     * @brief Returns a view of the substring [fpos, fpos + fcount), where fcount is a smaller of `count` and `size() - pos`.
     *
     * @throw std::out_of_range if pos > size()
     *
     * @param pos position of the first character.
     * @param count requested length.
     * @return View of the substring [fpos, fpos + fcount).
     */
    constexpr string_view substr(std::size_t pos, std::size_t count = npos) const {
        if (pos > size_)
            throw std::out_of_range("string_view::substr out_of_range");
        if (pos + count > size_)
            count = size_ - pos;
        return string_view(data_ + pos, count);
    }

    /**
     * @brief Returns a view of the substring (0, fcount], where fcount is a smaller of `count` and `size()`.
     *
     * @param count number of characters in prefix.
     *
     * @return View of the substring (0, fcount].
     */
    constexpr string_view prefix(std::size_t count) const noexcept {
        if (count > size_)
            count = size_;
        return string_view{data_, count};
    }

    /**
     * @brief Returns a view of the substring (size() - fcount, fcount], where fcount is a smaller of `count` and `size()`.
     *
     * @param count number of characters in suffix.
     *
     * @return View of the substring (0, fcount].
     */
    constexpr string_view suffix(std::size_t count) const noexcept {
        if (count > size_)
            count = size_;
        return string_view{data_ + size_ - count, count};
    }

    /**
     * @brief Moves the start of the view forward by fcount characters, where fcount is a smaller of `count` and `size()`.
     *
     * @param count number of characters to remove from the start of the view.
     */
    constexpr void remove_prefix(std::size_t count) noexcept {
        if (count > size_)
            count = size_;
        data_ += count;
        size_ -= count;
    }

    /**
     * @brief Moves the end of the view back by fcount characters, where fcount is a smaller of `count` and `size()`.
     *
     * @param count number of characters to remove from the end of the view.
     */
    constexpr void remove_suffix(std::size_t count) noexcept {
        if (count > size_)
            count = size_;
        size_ -= count;
    }

    /**
     * @brief Checks if the string view begins with the given prefix
     *
     * @param ch a single character
     * @return `true` if the string view begins with the provided prefix, `false` otherwise
     */
    constexpr bool starts_with(char ch) const noexcept { return size() > 0 && front() == ch; }

    /**
     * @brief Checks if the string view begins with the given prefix
     *
     * @param s a string view
     * @return `true` if the string view begins with the provided prefix< `false` otherwise
     */
    constexpr bool starts_with(string_view s) const noexcept { return prefix(s.size()) == s; }

    /**
     * @brief Checks if the string view ends with the given suffix
     *
     * @param ch a single character
     * @return `true` if the string view ends with the provided suffix, `false` otherwise.
     */
    constexpr bool ends_with(char ch) const noexcept { return size() > 0 && back() == ch; }

    /**
     * @brief Checks if the string view ends with the given suffix
     *
     * @param s a string view
     * @return `true` if the string view ends with the provided suffix, `false` otherwise.
     */
    constexpr bool ends_with(string_view s) const noexcept { return suffix(s.size()) == s; }

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

/**
 * @brief Null-terminated string_view.
 * Constucts from const char* and std::string only, default constructor equals to zstring_view("").
 */
class zstring_view : public string_view {
public:
    /**
     * @brief Default constructor equals to zstring_view("")
     * @post
     * size() == 0
     * *data() == '\0'
     * *c_str() == '\0'
     */
    constexpr zstring_view() noexcept
      : zstring_view("") {}

    /// Construct from null-terminated string.
    /// @post
    /// data() == cstr
    /// c_str() == cstr
    /// size() == (cstr == nullptr? std::strlen(cstr): 0)
    constexpr zstring_view(const char* cstr) noexcept
      : string_view(cstr) {}

    /// Construct from std::string
    /// @post
    /// data() == src.c_str()
    /// size() == src.size()
    /// c_str() == src.c_str()
    /// (*this)[size()] == '\0'
    zstring_view(const std::string& src) noexcept
      : string_view(src.c_str(), src.size()) {}

    zstring_view(std::nullptr_t) = delete;
    zstring_view(string_view)    = delete;

    /**
     * @brief Returns a pointer to an array that contains a null-terminated sequence of characters (i.e., a C-string)
     *
     * @return a pointer to an array that contains a null-terminated sequence of characters (i.e., a C-string)
     */
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

constexpr bool operator<(string_view lha, string_view rha) noexcept { return lha.compare(rha) == -1; }
constexpr bool operator<=(string_view lha, string_view rha) noexcept { return lha.compare(rha) != 1; }
constexpr bool operator>(string_view lha, string_view rha) noexcept { return lha.compare(rha) == 1; }
constexpr bool operator>=(string_view lha, string_view rha) noexcept { return lha.compare(rha) != -1; }

} // namespace tmdesc
