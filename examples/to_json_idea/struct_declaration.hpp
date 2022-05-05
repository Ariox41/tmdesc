// // Copyright Victor Smirnov 2021-2022
// // Distributed under the Boost Software License, Version 1.0.
// // (See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)

// #pragma once
// #include <string>
// #include <tmdesc/type_description.hpp> // for description only, almost no dependencies
// #include <vector>

// namespace flags {
// struct AdditionInfoTag {};
// constexpr tmdesc::flag<AdditionInfoTag, tmdesc::string_view> addition_info_flag(tmdesc::string_view info) noexcept {
//     return {info};
// }
// } // namespace flags

// namespace geometry {
// struct Point3 {
//     double x;
//     double y;
//     double z;
// };

// template <class Impl> constexpr auto tmdesc_info(tmdesc::type_t<Point3>, tmdesc::info_builder<Impl> builder) {
//     return builder(builder.member("x", &Point3::x), //
//                    builder.member("y", &Point3::y), //
//                    builder.member("z", &Point3::z)  //
//     );
// }
// } // namespace geometry

// struct Curve {
//     std::string name;
//     std::vector<geometry::Point3> points;
// };

// template <class Impl> constexpr auto tmdesc_info(tmdesc::type_t<Curve>, tmdesc::info_builder<Impl> build) {
//     return build(build.member("name", &Curve::name),                                //
//                  build.member("points", &Curve::points,                             //
//                               build.flags(flags::addition_info_flag("a 3D curve"))) //
//     );
// }
