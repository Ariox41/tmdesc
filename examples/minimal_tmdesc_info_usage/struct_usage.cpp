// Copyright Victor Smirnov 2021
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)

#include "iostream"
#include "struct_declaration.hpp"
#include "tmdesc/members_visitation.hpp"

std::ostream& operator<<(std::ostream& out, tmdesc::string_view str) {
    out.write(str.data(), str.size());
    return out;
}

struct print_visitor {

    template <class T, std::enable_if_t<tmdesc::has_type_info_v<T>, bool> = true> void operator()(const T& s) const {
        tmdesc::visit_members(s, [&](auto member_ref) {
            constexpr auto info = member_ref.flags().find_flag(tmdesc::type_t<flags::AdditionInfoTag>{});
            std::cout << member_ref.name();
            info.if_some([&](auto str) { std::cout << ", " << str; });
            std::cout << ": ";
            (*this)(member_ref.get());
            std::cout << "\n";
        });
    }

    template <class T> void operator()(const std::vector<T>& vec) const {
        std::cout << "[";
        for (const auto& item : vec) {
            (*this)(item);
            std::cout << ", ";
        }
        std::cout << "]";
    }

    void operator()(double value) const { std::cout << value; }
    void operator()(const std::string& value) const { std::cout << value; }
};

int main() {
    Curve curve{{"curve 1", ""}};

    print_visitor vis;
    vis(curve);
    return 0;
}
