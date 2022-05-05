#include "test_helpers.hpp"
#include <doctest/doctest.h>
#include <string>
//#include <tmdesc/foldable_struct.hpp>
#include <tmdesc/tmdesc_fwd.hpp>
#include <tmdesc/type_info/type_info_of.hpp>

namespace hana = boost::hana;
namespace ns {
struct s1 {
    int m0;
    char m1;
    unsigned int m2;
};

template <class Impl> constexpr auto tmdesc_info(tmdesc::info_builder<s1, Impl> builder) {
    return builder.type(                                //
        builder.members(builder.member("1", &s1::m0),   //
                        builder.member("10", &s1::m1),  //
                        builder.member("100", &s1::m2)) //
    );
}
} // namespace ns

TEST_CASE("tuple algorithm for struct") {
    // SUBCASE("simple get") {
    //     constexpr ns::s1 v1{41, '3', 56};

    //     STATIC_NOTHROW_CHECK(41 == hana::at_c<0>(v1));
    //     STATIC_NOTHROW_CHECK('3' ==  hana::at_c<1>(v1));
    //     STATIC_NOTHROW_CHECK(56 ==  hana::at_c<1>(v1));
    // }
    SUBCASE("for_each struct member") {
        // constexpr ns::s1 v{41, '3', 56};
        // constexpr auto ti = hana::sfinae(tmdesc::detail::type_info_of)(hana::type_c<ns::s1>);
        // STATIC_CHECK(!hana::is_nothing(ti));
        // constexpr auto mi = hana::chain(ti, tmdesc::detail::get_members);
        // STATIC_CHECK(!hana::is_nothing(mi));
        // constexpr auto mi0 = mi.value()[hana::size_c<0>];
        // STATIC_CHECK(mi0.name() == "1");
        // STATIC_CHECK(mi0.getter()(v) == 41);
  

        //  STATIC_CHECK(tmdesc::type_info_of(hana::type_c<ns::s1>) != hana::nothing);
        //STATIC_CHECK_AND_POSSIBLE_NOTHROW(hana::fold_left(v, 0, hana::plus) == (41 + '3' + 56));
    }
}
