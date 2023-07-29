#include "test_helpers.hpp"
#include "test_structs_description.hpp"
#include <boost/hana/fold.hpp>
#include <boost/hana/size.hpp>
#include <functional>
#include <string>
#include <tmdesc/struct.hpp>

namespace hana = boost::hana;
TEST_CASE("struct unpack") {
    SUBCASE("struct_with_typename_only") {
        using TS = ns::struct_with_typename_only;
        STATIC_CHECK(tmdesc::FoldableStruct<TS>{});
        STATIC_CHECK(hana::Foldable<TS>{});
        TS s{};
        constexpr auto size = hana::size(s);
        STATIC_CHECK(size == hana::size_c<0>);
    }
    SUBCASE("single_value_without_attributes") {
        using TS = ns::single_value_without_attributes;
        STATIC_CHECK(tmdesc::FoldableStruct<TS>{});
        STATIC_CHECK(hana::Foldable<TS>{});
        TS s{};
        constexpr auto size = hana::size(s);
        STATIC_CHECK(size == hana::size_c<1>);
    }
    SUBCASE("multiply_values_without_attributes") {
        using TS = ns::multiply_values_without_attributes;
        STATIC_CHECK(tmdesc::FoldableStruct<TS>{});
        STATIC_CHECK(hana::Foldable<TS>{});
        TS s{};
        constexpr auto size = hana::size(s);
        STATIC_CHECK(size == hana::size_c<3>);
    }
    SUBCASE("multiple_values_with_attributes") {
        using TS = ns::multiple_values_with_attributes;
        STATIC_CHECK(tmdesc::FoldableStruct<TS>{});
        STATIC_CHECK(hana::Foldable<TS>{});
        constexpr TS s{-45, '1', 11};
        constexpr auto size = hana::size(s);
        STATIC_CHECK(size == hana::size_c<3>);
        STATIC_CHECK(hana::is_just(tmdesc::get_type_members_info(hana::typeid_(s))));

        STATIC_CHECK(hana::equal(s, s));
        STATIC_CHECK(!hana::equal(s, TS{1, '2', 3}));
        STATIC_CHECK(hana::fold(s, std::plus<int>{}) == -45 + '1' + 11);
    }
}
