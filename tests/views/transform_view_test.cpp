#include "../test_helpers.hpp"
#include <tmdesc/views/transform_view.hpp>

TEST_SUITE("ref_obj cast") {
    TEST_CASE("ref_obj<T> to ref_obj<const T>") {
        int v{};
        tmdesc::ref_obj<int> refMut{v};
        auto clonedRefMut = tmdesc::ref_cast<int>(refMut);
        static_assert(std::is_same<decltype(clonedRefMut), tmdesc::ref_obj<int>>::value, "");

        auto refConst = tmdesc::ref_cast<const int>(refMut);
        static_assert(std::is_same<decltype(refConst), tmdesc::ref_obj<const int>>::value, "");

        auto refAutoConst = tmdesc::ref_cast<int>(refConst);
        static_assert(std::is_same<decltype(refAutoConst), tmdesc::ref_obj<const int>>::value, "");
    }
}