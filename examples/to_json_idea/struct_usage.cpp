// Copyright Victor Smirnov 2021
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)

#include "struct_declaration.hpp"
#include <iostream>
#include <tmdesc/members_view.hpp>
#include <tmdesc/algorithm/for_each.hpp>

std::ostream& operator<<(std::ostream& out, tmdesc::string_view str) {
    out.write(str.data(), str.size());
    return out;
}

struct print_visitor {
    template <class T, std::enable_if_t<tmdesc::has_type_info_v<T>, bool> = true> //
    void operator()(const T& s) const {
        std::cout << "{";
        tmdesc::string_view separator = "";
        tmdesc::for_each(tmdesc::members_view(s), [&](auto member_ref) {
            std::cout << separator;
            separator = ", ";

            constexpr auto name = member_ref.name();
            std::cout << '"' << name;
            constexpr auto info = member_ref.flags().find_flag(tmdesc::type_t<flags::AdditionInfoTag>{});
            info.if_some([&](auto str) { std::cout << "[" << str << "]"; });
            std::cout << R"(": )";
            (*this)(member_ref.get());
        });
        std::cout << "}";
    }

    template <class T> void operator()(const std::vector<T>& vec) const {
        std::cout << "[";
        tmdesc::string_view separator = "";
        for (const auto& item : vec) {
            std::cout << separator;
            separator = ", ";
            (*this)(item);
        }
        std::cout << "]";
    }

    void operator()(double value) const { std::cout << value; }
    void operator()(const std::string& value) const { std::cout << '"' << value << '"'; }
};

int main() {
    Curve curve{std::string{"curve 1"}, {geometry::Point3{1., 2., 3.}, geometry::Point3{42.5, 0.1, 13.}}};

    print_visitor print;
    print(curve);
    std::cout << std::endl;
    return 0;
}
