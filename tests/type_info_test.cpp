#include "test_helpers.hpp"
#include "test_structs_description.hpp"
#include <boost/hana/size.hpp>
#include <doctest/doctest.h>
#include <string>
#include <tmdesc/type_info/type_info_of.hpp>

namespace hana = boost::hana;

TEST_CASE("type info of") {
    SUBCASE("struct_with_typename_only") {
        // the value is not constexpr, but the information is constexpr
        ns::struct_with_typename_only value{};

        constexpr auto&& ti = tmdesc::type_info_of(hana::typeid_(value));
        STATIC_CHECK_AND_POSSIBLE_NOTHROW(!(ti == hana::nothing));

        constexpr auto&& ta = tmdesc::type_attributes_of(hana::typeid_(value));
        STATIC_CHECK_AND_POSSIBLE_NOTHROW(!(ta == hana::nothing));

        constexpr auto type_name =
            hana::chain(ta, hana::reverse_partial(hana::find, hana::type_c<tmdesc::tags::type_name>));
        STATIC_CHECK_AND_POSSIBLE_NOTHROW(type_name == hana::just(tmdesc::zstring_view("struct_with_typename_only")));

        constexpr auto&& tm = tmdesc::type_members_info_of(hana::typeid_(value));
        STATIC_CHECK_AND_POSSIBLE_NOTHROW(tm == hana::nothing);
    }
    SUBCASE("empty_struct_with_typename") {
        // the value is not constexpr, but the information is constexpr
        ns::empty_struct_with_typename value{};

        constexpr auto&& ti = tmdesc::type_info_of(hana::typeid_(value));
        STATIC_CHECK_AND_POSSIBLE_NOTHROW(!(ti == hana::nothing));

        constexpr auto&& ta = tmdesc::type_attributes_of(hana::typeid_(value));
        STATIC_CHECK_AND_POSSIBLE_NOTHROW(!(ta == hana::nothing));

        constexpr auto type_name =
            hana::chain(ta, hana::reverse_partial(hana::find, hana::type_c<tmdesc::tags::type_name>));
        STATIC_CHECK_AND_POSSIBLE_NOTHROW(type_name == hana::just(tmdesc::zstring_view("empty_struct_with_typename")));

        constexpr auto&& tm = tmdesc::type_members_info_of(hana::typeid_(value));
        STATIC_CHECK_AND_POSSIBLE_NOTHROW(tm == hana::just(hana::make_tuple()));
    }
    SUBCASE("single_value_without_attributes") {
        // the value is not constexpr, but the information is constexpr
        ns::single_value_without_attributes value{"test_value"};

        constexpr auto&& ti = tmdesc::type_info_of(hana::typeid_(value));
        STATIC_CHECK_AND_POSSIBLE_NOTHROW(!(ti == hana::nothing));

        constexpr auto&& ta = tmdesc::type_attributes_of(hana::typeid_(value));
        STATIC_CHECK_AND_POSSIBLE_NOTHROW(ta == hana::just(hana::make_map()));

        constexpr auto&& tm = tmdesc::type_members_info_of(hana::typeid_(value));
        STATIC_CHECK_AND_POSSIBLE_NOTHROW(hana::transform(tm, hana::size) == hana::just(hana::size_c<1>));

        constexpr auto&& m0 = tm.value()[hana::size_c<0>];
        STATIC_CHECK_AND_POSSIBLE_NOTHROW(m0.name() == "member");
        STATIC_CHECK_AND_POSSIBLE_NOTHROW(m0.attributes() == hana::make_map());
        REQUIRE(m0.getter()(value) == "test_value");
    }
    SUBCASE("multiply_values_without_attributes") {
        // the value is constexpr
        constexpr ns::multiply_values_without_attributes value{-1, char(10), 100};

        constexpr auto&& ti = tmdesc::type_info_of(hana::typeid_(value));
        STATIC_CHECK_AND_POSSIBLE_NOTHROW(!(ti == hana::nothing));

        constexpr auto&& ta = tmdesc::type_attributes_of(hana::typeid_(value));
        STATIC_CHECK_AND_POSSIBLE_NOTHROW(ta == hana::just(hana::make_map()));

        constexpr auto&& tm = tmdesc::type_members_info_of(hana::typeid_(value));
        STATIC_CHECK_AND_POSSIBLE_NOTHROW(hana::transform(tm, hana::size) == hana::just(hana::size_c<3>));

        constexpr auto&& m0 = tm.value()[hana::size_c<0>];
        constexpr auto&& m1 = tm.value()[hana::size_c<1>];
        constexpr auto&& m2 = tm.value()[hana::size_c<2>];
        STATIC_CHECK_AND_POSSIBLE_NOTHROW(m0.name() == "1");
        STATIC_CHECK_AND_POSSIBLE_NOTHROW(m1.name() == "10");
        STATIC_CHECK_AND_POSSIBLE_NOTHROW(m2.name() == "100");
        STATIC_CHECK_AND_POSSIBLE_NOTHROW(m0.type_id() == hana::type_c<int>);
        STATIC_CHECK_AND_POSSIBLE_NOTHROW(m1.type_id() == hana::type_c<char>);
        STATIC_CHECK_AND_POSSIBLE_NOTHROW(m2.type_id() == hana::type_c<unsigned>);
        STATIC_CHECK_AND_POSSIBLE_NOTHROW(m0.attributes() == hana::make_map());
        STATIC_CHECK_AND_POSSIBLE_NOTHROW(m1.attributes() == hana::make_map());
        STATIC_CHECK_AND_POSSIBLE_NOTHROW(m2.attributes() == hana::make_map());
        STATIC_CHECK_AND_POSSIBLE_NOTHROW(m0.getter()(value) == -1);
        STATIC_CHECK_AND_POSSIBLE_NOTHROW(m1.getter()(value) == 10);
        STATIC_CHECK_AND_POSSIBLE_NOTHROW(m2.getter()(value) == 100);
    }
    SUBCASE("multiple_values_with_attributes") {
        // the value is constexpr
        constexpr ns::multiple_values_with_attributes value{-1, char(10), 100};

        constexpr auto&& ti = tmdesc::type_info_of(hana::typeid_(value));
        STATIC_CHECK_AND_POSSIBLE_NOTHROW(!hana::is_nothing(ti));

        constexpr auto&& ta = tmdesc::type_attributes_of(hana::typeid_(value));

        constexpr auto type_name =
            hana::chain(ta, hana::reverse_partial(hana::find, hana::type_c<tmdesc::tags::type_name>));
        STATIC_CHECK_AND_POSSIBLE_NOTHROW(type_name == hana::just("multiple_values_with_attributes"));

        constexpr auto&& tm = tmdesc::type_members_info_of(hana::typeid_(value));
        STATIC_CHECK_AND_POSSIBLE_NOTHROW(hana::transform(tm, hana::size) == hana::just(hana::size_c<3>));

        constexpr auto&& m0 = tm.value()[hana::size_c<0>];
        constexpr auto&& m1 = tm.value()[hana::size_c<1>];
        constexpr auto&& m2 = tm.value()[hana::size_c<2>];
        STATIC_CHECK_AND_POSSIBLE_NOTHROW(m0.name() == "M0");
        STATIC_CHECK_AND_POSSIBLE_NOTHROW(m1.name() == "M1");
        STATIC_CHECK_AND_POSSIBLE_NOTHROW(m2.name() == "M2");

        STATIC_CHECK_AND_POSSIBLE_NOTHROW(m0.type_id() == hana::type_c<int>);
        STATIC_CHECK_AND_POSSIBLE_NOTHROW(m1.type_id() == hana::type_c<char>);
        STATIC_CHECK_AND_POSSIBLE_NOTHROW(m2.type_id() == hana::type_c<unsigned>);

        STATIC_CHECK_AND_POSSIBLE_NOTHROW(m0.attributes() == hana::make_map());
        STATIC_CHECK_AND_POSSIBLE_NOTHROW(m1.attributes() == hana::make_map());
        STATIC_CHECK_AND_POSSIBLE_NOTHROW(hana::size(m2.attributes()) == hana::size_c<1>);

        constexpr auto in_lowercase = hana::find(m2.attributes(), hana::type_c<ns::tags::name_in_lowercase>);
        STATIC_CHECK_AND_POSSIBLE_NOTHROW(in_lowercase == hana::just(true));

        STATIC_CHECK_AND_POSSIBLE_NOTHROW(m0.getter()(value) == -1);
        STATIC_CHECK_AND_POSSIBLE_NOTHROW(m1.getter()(value) == 10);
        STATIC_CHECK_AND_POSSIBLE_NOTHROW(m2.getter()(value) == 100);
    }
}