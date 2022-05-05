#pragma once

#include <tmdesc/tmdesc_fwd.hpp>
namespace ns {
struct struct_with_typename_only {
    int undescribed_member = 42;

    template <class Impl>
    friend constexpr auto tmdesc_info(tmdesc::info_builder<struct_with_typename_only, Impl> builder) {
        return builder.type(                                                   //
            builder.attributes(builder.type_name("struct_with_typename_only")) //
        );
    }
};

struct empty_struct_with_typename {
    template <class Impl>
    friend constexpr auto tmdesc_info(tmdesc::info_builder<empty_struct_with_typename, Impl> builder) {
        return builder.type(                                                     //
            builder.attributes(builder.type_name("empty_struct_with_typename")), //
            builder.members()                                                    //
        );
    }
};

struct single_value_without_attributes {
    const char* member = "value";
    template <class Impl>
    friend constexpr auto tmdesc_info(tmdesc::info_builder<single_value_without_attributes, Impl> builder) {
        using type = single_value_without_attributes;
        return builder.type(                                         //
            builder.members(builder.member("member", &type::member)) //
        );
    }
};

struct multiply_values_without_attributes {
    int m0;
    char m1;
    unsigned int m2;
};

template <class Impl>
constexpr auto tmdesc_info(tmdesc::info_builder<multiply_values_without_attributes, Impl> builder) {
    using type = multiply_values_without_attributes;
    return builder.type(                                  //
        builder.members(builder.member("1", &type::m0),   //
                        builder.member("10", &type::m1),  //
                        builder.member("100", &type::m2)) //
    );
}

namespace tags {

struct name_in_lowercase {};

constexpr ::tmdesc::attribute<name_in_lowercase, bool> make_name_in_lowercase(bool b = true) { return {b}; }

} // namespace tags

struct multiple_values_with_attributes {
    int m0;
    char m1;
    unsigned int m2;

    template <class Impl>
    friend constexpr auto tmdesc_info(tmdesc::info_builder<multiple_values_with_attributes, Impl> builder) {
        using self_t = multiple_values_with_attributes;
        return builder.type(
            builder.attributes(builder.type_name("multiple_values_with_attributes")),                             //
            builder.members(builder.member("M0", &self_t::m0, builder.attributes()),                              //
                            builder.member("M1", &self_t::m1),                                                    //
                            builder.member("M2", &self_t::m2, builder.attributes(tags::make_name_in_lowercase())) //
                            ));
    }
};

} // namespace ns