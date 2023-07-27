#pragma once

#include <tmdesc/tmdesc_fwd.hpp>
namespace ns {
using tmdesc::attributes::inherits;
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

namespace attributes {

struct name_in_lowercase_tag {
    constexpr ::tmdesc::attribute<name_in_lowercase_tag, bool> operator()(bool b = true) const { return {b}; }
};

inline constexpr name_in_lowercase_tag name_in_lowercase{};

} // namespace attributes

struct multiple_values_with_attributes {
    int m1;
    char m2;
    unsigned int m3;

    template <class Impl>
    friend constexpr auto tmdesc_info(tmdesc::info_builder<multiple_values_with_attributes, Impl> type) {
        using Self = multiple_values_with_attributes;
        return type.build(type.attributes(type_name("multiple_values_with_attributes")),             //
                          type.members(type.member("M1", &Self::m1, type.attributes()),              //
                                       type.member("M2", &Self::m2),                                 //
                                       type.member("M3", &Self::m3, attributes::name_in_lowercase()) //
                                       ));
    }
};

struct inheritor : multiple_values_with_attributes {
    int m4;
    template <class Impl> friend constexpr auto tmdesc_info(tmdesc::info_builder<inheritor, Impl> type) {
        using Self = inheritor;
        return type.build(
            type.attributes(type_name("inheritor"), inherits<multiple_values_with_attributes>), //
            type.members(type.member("M4", &Self::m4, type_name("int"), attributes::name_in_lowercase())));
    }
};

struct transparent_aggregation {
    int m0;
    inheritor transparent;
    int m5;
    int m6;
    template <class Impl> friend constexpr auto tmdesc_info(tmdesc::info_builder<transparent_aggregation, Impl> type) {
        using Self = transparent_aggregation;
        return type.build(type.attributes(type_name("transparent_aggregation"),                                 //
                                          tmdesc::attributes::flatten_transparent()),                           //
                          type.members(type.member("M0", &Self::m0),                                            //
                                       type.member("_", &Self::transparent, tmdesc::attributes::transparent()), //
                                       type.member("_", &Self::m5, tmdesc::attributes::transparent()),          //
                                       type.member("M6", &Self::m6, attributes::name_in_lowercase())));
    }
};
} // namespace ns
