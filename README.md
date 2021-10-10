# tmdesc
Serialisation-oriented type members description library for c++ 14

Work in progress

# Usage:
#### struct_declaration.hpp
``` c++
#pragma once
#include <string>
#include <tmdesc/type_description.hpp> // for description only, almost no dependencies
#include <vector>

namespace flags {
struct AdditionInfoTag {};
constexpr tmdesc::flag<AdditionInfoTag, tmdesc::string_view> addition_info_flag(tmdesc::string_view info) noexcept {
    return {info};
}
} // namespace flags

namespace geometry {
struct Point3 {
    double x;
    double y;
    double z;
};

template <class Impl> constexpr auto tmdesc_info(tmdesc::type_t<Point3>, tmdesc::info_builder<Impl> builder) {
    return builder(builder.member("x", &Point3::x), //
                   builder.member("y", &Point3::y), //
                   builder.member("z", &Point3::z)  //
    );
}
} // namespace geometry

struct Curve {
    std::string name;
    std::vector<geometry::Point3> points;
};

template <class Impl> constexpr auto tmdesc_info(tmdesc::type_t<Curve>, tmdesc::info_builder<Impl> build) {
    return build(build.member("name", &Curve::name),                                //
                 build.member("points", &Curve::points,                             //
                              build.flags(flags::addition_info_flag("a 3D curve"))) //
    );
}

```

#### struct_usage.cpp
``` c++
#include "struct_declaration.hpp"
#include <iostream>
#include <tmdesc/members_visitation.hpp>

std::ostream& operator<<(std::ostream& out, tmdesc::string_view str) {
    out.write(str.data(), str.size());
    return out;
}

struct print_visitor {
    template <class T, std::enable_if_t<tmdesc::has_type_info_v<T>, bool> = true> //
    void operator()(const T& s) const {
        std::cout << "{";
        tmdesc::string_view separator = "";
        tmdesc::visit_members(s, [&](auto member_ref) {
            std::cout << separator;
            separator = ", ";

            std::cout << '"' << member_ref.name();
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
```
**output**: 
```json
{"name": "curve 1", "points[a 3D curve]": [{"x": 1, "y": 2, "z": 3}, {"x": 42.5, "y": 0.1, "z": 13}]}
```
