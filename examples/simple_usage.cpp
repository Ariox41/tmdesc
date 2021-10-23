// Copyright Victor Smirnov 2021
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)

#include <iostream>
#include <tmdesc/algorithm/for_each.hpp>
#include <tmdesc/members_view.hpp>
#include <tmdesc/type_description.hpp>

// Custom member flag tag
struct additional_description_tag;

// Custom member flag implementation
constexpr tmdesc::flag<additional_description_tag, tmdesc::zstring_view>
additional_description(tmdesc::zstring_view descr) {
    return {descr};
}

namespace custom_ns {
struct Point {
    int x;
    int y;
};

// members description in free function template
template <class Impl>
constexpr auto tmdesc_info(::tmdesc::type_t<Point>, ::tmdesc::info_builder<Impl> wrap) {
    return wrap(wrap.member("x", &Point::x), //
                wrap.member("y", &Point::y));
}

struct Rect {
    Point top_left;
    Point bottom_right;

    // members description in friend function template
    template <class Impl>
    friend constexpr auto tmdesc_info(::tmdesc::type_t<Rect>, ::tmdesc::info_builder<Impl> wrap) {
        return wrap(wrap.member("tl", &Rect::top_left),
                    wrap.member("br", &Rect::bottom_right,
                                // custom flag for member
                                wrap.flags(additional_description("br point description"))));
    }
};
} // namespace custom_ns

void print(int v) { std::cout << v; }
void print(tmdesc::zstring_view str) { std::cout << "\"" << str.c_str() << "\""; }

template <class T, std::enable_if_t<tmdesc::has_type_info_v<T>, bool> = true>
void print(const T& object_with_description) {
    std::cout << "{ ";
    tmdesc::zstring_view separator = "";
    ::tmdesc::for_each(tmdesc::members_view(object_with_description), [&](auto member) {
        std::cout << separator.c_str() << member.name().c_str() << ": { ";

        // Using a custom flag if it exists
        auto&& descr = member.flags().find_flag(tmdesc::type_t<additional_description_tag>{});
        descr.if_some(
            [&](auto descr) { std::cout << "description: \"" << descr.c_str() << "\", "; });

        std::cout << "value: ";
        print(member.get()); // get reference to object member, const reference in this case
        std::cout << " }";

        separator = ", ";
    });
    std::cout << " }";
}

int main() {
    custom_ns::Rect rect{{42, 20}, {3, -11}};
    print(rect);
    std::cout << std::endl;
    return 0;
}
