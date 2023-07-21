#include "test_helpers.hpp"
#include "test_structs_description.hpp"
#include <boost/hana/not_equal.hpp>
#include <boost/hana/size.hpp>
#include <doctest/doctest.h>
#include <string>
#include <tmdesc/functional/invoke.hpp>
#include <tmdesc/get_type_info.hpp>

namespace hana = boost::hana;

template <class T> struct check_type;

TEST_SUITE("type name info") {

    using tmdesc::attributes::type_name_tag;
    TEST_CASE("struct_with_typename_only") {
        ns::struct_with_typename_only value{};

        constexpr auto ti = tmdesc::get_type_info(hana::typeid_(value));
        STATIC_CHECK(ti != hana::nothing);

        constexpr auto ta = tmdesc::get_type_attributes(hana::typeid_(value));
        STATIC_CHECK(ta != hana::nothing);

        constexpr auto type_name = tmdesc::find_type_attribute(hana::typeid_(value), hana::type_c<type_name_tag>);

        STATIC_CHECK(type_name == hana::just(tmdesc::zstring_view("struct_with_typename_only")));

        constexpr auto tm = tmdesc::get_type_members_info(hana::typeid_(value));
        STATIC_CHECK(tm == hana::just(hana::make_basic_tuple()));
    }

    TEST_CASE("single_value_without_attributes") {
        ns::single_value_without_attributes value{"test_value"};

        constexpr auto ti = tmdesc::get_type_info(hana::typeid_(value));
        STATIC_CHECK(ti != hana::nothing);

        constexpr auto ta = tmdesc::get_type_attributes(hana::typeid_(value));
        STATIC_CHECK(ta != hana::nothing);

        constexpr auto tm = tmdesc::get_type_members_info(hana::typeid_(value));
        STATIC_CHECK(hana::transform(tm, hana::size) == hana::just(hana::size_c<1>));

        constexpr auto m0 = hana::at(tm.value(), hana::size_c<0>);
        STATIC_CHECK(m0.name() == "member");
        STATIC_CHECK(m0.attributes() == hana::make_map());
        REQUIRE(tmdesc::invoke(m0.accessor(), value) == "test_value");
    }
    TEST_CASE("multiply_values_without_attributes") {
        constexpr ns::multiply_values_without_attributes value{-1, char(10), 100};

        constexpr auto ti = tmdesc::get_type_info(hana::typeid_(value));
        STATIC_CHECK(ti != hana::nothing);

        constexpr auto ta = tmdesc::get_type_attributes(hana::typeid_(value));
        STATIC_CHECK(ta == hana::just(hana::make_map()));

        constexpr auto tm = tmdesc::get_type_members_info(hana::typeid_(value));
        STATIC_CHECK(hana::transform(tm, hana::size) == hana::just(hana::size_c<3>));

        constexpr auto m0 = hana::at_c<0>(tm.value());
        constexpr auto m1 = hana::at_c<1>(tm.value());
        constexpr auto m2 = hana::at_c<2>(tm.value());
        STATIC_CHECK(m0.name() == "1");
        STATIC_CHECK(m1.name() == "10");
        STATIC_CHECK(m2.name() == "100");
        STATIC_CHECK(m0.attributes() == hana::make_map());
        STATIC_CHECK(m1.attributes() == hana::make_map());
        STATIC_CHECK(m2.attributes() == hana::make_map());
        STATIC_CHECK(tmdesc::invoke(m0.accessor(), value) == -1);
        STATIC_CHECK(tmdesc::invoke(m1.accessor(), value) == 10);
        STATIC_CHECK(tmdesc::invoke(m2.accessor(), value) == 100);
    }
    TEST_CASE("multiple_values_with_attributes") {
        constexpr ns::multiple_values_with_attributes value{-1, char(10), 100};

        constexpr auto ti = tmdesc::get_type_info(hana::typeid_(value));
        STATIC_CHECK(ti != hana::nothing);

        constexpr auto ta = tmdesc::get_type_attributes(hana::typeid_(value));
        STATIC_CHECK(ta != hana::nothing);

        constexpr auto type_name = tmdesc::find_type_attribute(hana::typeid_(value), hana::type_c<type_name_tag>);
        STATIC_CHECK(type_name == hana::just(tmdesc::zstring_view("multiple_values_with_attributes")));

        constexpr auto tm = tmdesc::get_type_members_info(hana::typeid_(value));
        STATIC_CHECK(hana::transform(tm, hana::size) == hana::just(hana::size_c<3>));

        constexpr auto m1 = hana::at_c<0>(tm.value());
        constexpr auto m2 = hana::at_c<1>(tm.value());
        constexpr auto m3 = hana::at_c<2>(tm.value());
        STATIC_CHECK(m1.name() == "M1");
        STATIC_CHECK(m2.name() == "M2");
        STATIC_CHECK(m3.name() == "M3");

        STATIC_CHECK(m1.attributes() == hana::make_map());
        STATIC_CHECK(m2.attributes() == hana::make_map());
        STATIC_CHECK(hana::size(m3.attributes()) == hana::size_c<1>);

        constexpr auto lowercase = hana::find(m3.attributes(), hana::type_c<ns::attributes::name_in_lowercase_tag>);
        STATIC_CHECK(lowercase == hana::just(true));

        STATIC_CHECK(tmdesc::invoke(m1.accessor(), value) == -1);
        STATIC_CHECK(tmdesc::invoke(m2.accessor(), value) == 10);
        STATIC_CHECK(tmdesc::invoke(m3.accessor(), value) == 100);
    }

    TEST_CASE("inheritor") {
        constexpr ns::inheritor value{{-1, char(10), 100}, 1000};

        constexpr auto ti = tmdesc::get_type_info(hana::typeid_(value));
        STATIC_CHECK(ti != hana::nothing);

        constexpr auto ta = tmdesc::get_type_attributes(hana::typeid_(value));
        STATIC_CHECK(ta != hana::nothing);

        constexpr auto type_name = tmdesc::find_type_attribute(hana::typeid_(value), hana::type_c<type_name_tag>);
        STATIC_CHECK(type_name == hana::just(tmdesc::zstring_view("inheritor")));

        constexpr auto tm = tmdesc::get_type_members_info(hana::typeid_(value));
        STATIC_CHECK(hana::transform(tm, hana::size) == hana::just(hana::size_c<4>));

        constexpr auto m1 = hana::at_c<0>(tm.value());
        constexpr auto m2 = hana::at_c<1>(tm.value());
        constexpr auto m3 = hana::at_c<2>(tm.value());
        constexpr auto m4 = hana::at_c<3>(tm.value());
        STATIC_CHECK(m1.name() == "M1");
        STATIC_CHECK(m2.name() == "M2");
        STATIC_CHECK(m3.name() == "M3");
        STATIC_CHECK(m4.name() == "M4");

        STATIC_CHECK(m1.attributes() == hana::make_map());
        STATIC_CHECK(m2.attributes() == hana::make_map());

        STATIC_CHECK(hana::size(m3.attributes()) == hana::size_c<1>);
        constexpr auto lowercase = hana::find(m3.attributes(), hana::type_c<ns::attributes::name_in_lowercase_tag>);
        STATIC_CHECK(lowercase == hana::just(true));

        STATIC_CHECK(hana::size(m4.attributes()) == hana::size_c<2>);
        STATIC_CHECK(hana::find(m4.attributes(), hana::type_c<tmdesc::attributes::type_name_tag>) ==
                     hana::just(tmdesc::zstring_view("int")));
        STATIC_CHECK(hana::find(m4.attributes(), hana::type_c<ns::attributes::name_in_lowercase_tag>) ==
                     hana::just(true));

        STATIC_CHECK(tmdesc::invoke(m1.accessor(), value) == -1);
        STATIC_CHECK(tmdesc::invoke(m2.accessor(), value) == 10);
        STATIC_CHECK(tmdesc::invoke(m3.accessor(), value) == 100);
        STATIC_CHECK(tmdesc::invoke(m4.accessor(), value) == 1000);
    }

    TEST_CASE("transparent_aggregation") {
        constexpr ns::transparent_aggregation value{
            -10, ns::inheritor{ns::multiple_values_with_attributes{-1, char(10), 100}, 1000}, 0, 10000};

        constexpr auto ti = tmdesc::get_type_info(hana::typeid_(value));
        STATIC_CHECK(ti != hana::nothing);

        constexpr auto ta = tmdesc::get_type_attributes(hana::typeid_(value));
        STATIC_CHECK(ta != hana::nothing);

        constexpr auto type_name = tmdesc::find_type_attribute(hana::typeid_(value), hana::type_c<type_name_tag>);
        STATIC_CHECK(type_name == hana::just(tmdesc::zstring_view("transparent_aggregation")));

        constexpr auto tm = tmdesc::get_type_members_info(hana::typeid_(value));

        STATIC_CHECK(hana::transform(tm, hana::size) == hana::just(hana::size_c<6>));

        constexpr auto m0 = hana::at_c<0>(tm.value());
        constexpr auto m1 = hana::at_c<1>(tm.value());
        constexpr auto m2 = hana::at_c<2>(tm.value());
        constexpr auto m3 = hana::at_c<3>(tm.value());
        constexpr auto m4 = hana::at_c<4>(tm.value());
        constexpr auto m6 = hana::at_c<5>(tm.value());
        STATIC_CHECK(m0.name() == "M0");
        STATIC_CHECK(m1.name() == "M1");
        STATIC_CHECK(m2.name() == "M2");
        STATIC_CHECK(m3.name() == "M3");
        STATIC_CHECK(m4.name() == "M4");
        STATIC_CHECK(m6.name() == "M6");

        STATIC_CHECK(m0.attributes() == hana::make_map());
        STATIC_CHECK(m1.attributes() == hana::make_map());
        STATIC_CHECK(m2.attributes() == hana::make_map());

        STATIC_CHECK(hana::size(m3.attributes()) == hana::size_c<1>);
        constexpr auto lowercase = hana::find(m3.attributes(), hana::type_c<ns::attributes::name_in_lowercase_tag>);
        STATIC_CHECK(lowercase == hana::just(true));

        STATIC_CHECK(hana::size(m4.attributes()) == hana::size_c<2>);
        STATIC_CHECK(hana::find(m4.attributes(), hana::type_c<tmdesc::attributes::type_name_tag>) ==
                     hana::just(tmdesc::zstring_view("int")));
        STATIC_CHECK(hana::find(m4.attributes(), hana::type_c<ns::attributes::name_in_lowercase_tag>) ==
                     hana::just(true));

        STATIC_CHECK(hana::find(m6.attributes(), hana::type_c<ns::attributes::name_in_lowercase_tag>) ==
                     hana::just(true));

        STATIC_CHECK(tmdesc::invoke(m0.accessor(), value) == -10);
        STATIC_CHECK(tmdesc::invoke(m1.accessor(), value) == -1);
        STATIC_CHECK(tmdesc::invoke(m2.accessor(), value) == 10);
        STATIC_CHECK(tmdesc::invoke(m3.accessor(), value) == 100);
        STATIC_CHECK(tmdesc::invoke(m4.accessor(), value) == 1000);
        STATIC_CHECK(tmdesc::invoke(m6.accessor(), value) == 10000);
    }
}
