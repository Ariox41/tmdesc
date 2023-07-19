#pragma once

#include <tmdesc/tmdesc_fwd.hpp>
namespace ns {
using tmdesc::attributes::type_name;
struct struct_with_typename_only {
    int undescribed_member = 42;

    template <class Impl>
    friend constexpr auto tmdesc_info(tmdesc::info_builder<struct_with_typename_only, Impl> type) {
        return type.build(                                          //
            type.attributes(type_name("struct_with_typename_only")) //
        );
    }
};

struct single_value_without_attributes {
    const char* member = "value";
    template <class Impl>
    friend constexpr auto tmdesc_info(tmdesc::info_builder<single_value_without_attributes, Impl> type) {
        using Self = single_value_without_attributes;
        return type.build(                                     //
            type.members(type.member("member", &Self::member)) //
        );
    }
};

struct multiply_values_without_attributes {
    int m0;
    char m1;
    unsigned int m2;
};

template <class Impl> constexpr auto tmdesc_info(tmdesc::info_builder<multiply_values_without_attributes, Impl> type) {
    using Self = multiply_values_without_attributes;
    return type.build(                              //
        type.members(type.member("1", &Self::m0),   //
                     type.member("10", &Self::m1),  //
                     type.member("100", &Self::m2)) //
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
    friend constexpr auto tmdesc_info(tmdesc::info_builder<multiple_values_with_attributes, Impl> type) {
        using Self = multiple_values_with_attributes;
        return type.build(type.attributes(type_name("multiple_values_with_attributes")),                             //
                          type.members(type.member("M0", &Self::m0, type.attributes()),                              //
                                       type.member("M1", &Self::m1),                                                 //
                                       type.member("M2", &Self::m2, type.attributes(tags::make_name_in_lowercase())) //
                                       ));
    }
};

} // namespace ns
