#include "../test_helpers.hpp"
#include <tmdesc/functional/ref_obj.hpp>

TEST_SUITE("ref_obj cast") {
    TEST_CASE("ref_obj<T> to ref_obj<const T>") {
        int v = 42;
        tmdesc::ref_obj<int> refMut{v};
        auto clonedRefMut = tmdesc::ref_cast<int>(refMut);
        static_assert(std::is_same<decltype(clonedRefMut), tmdesc::ref_obj<int>>::value, "");
        REQUIRE(clonedRefMut == v);

        auto refConst = tmdesc::ref_cast<const int>(refMut);
        static_assert(std::is_same<decltype(refConst), tmdesc::ref_obj<const int>>::value, "");
        REQUIRE(refConst == v);

        auto refAutoConst = tmdesc::ref_cast<int>(refConst);
        static_assert(std::is_same<decltype(refAutoConst), tmdesc::ref_obj<const int>>::value, "");
        REQUIRE(refAutoConst == v);
    }
    TEST_CASE("ref_obj<Derived> to ref_obj<Base>") {
        struct Base {
            int value;
        };
        struct Derived : Base {};

        Derived derived{};
        derived.value = 42;
        tmdesc::ref_obj<Derived> refDerived{derived};

        auto refBase = tmdesc::ref_cast<Base>(refDerived);
        static_assert(std::is_same<decltype(refBase), tmdesc::ref_obj<Base>>::value, "");
        REQUIRE(refBase.get().value == 42);

        auto refConstBase = tmdesc::ref_cast<const Base>(refDerived);
        static_assert(std::is_same<decltype(refConstBase), tmdesc::ref_obj<const Base>>::value, "");
        REQUIRE(refConstBase.get().value == 42);
    }
}
TEST_CASE("pointer to ref_obj") {
    int v{42};
    int* ptr    = &v;
    auto refObj = tmdesc::inderect_ref(ptr);
    static_assert(std::is_same<decltype(refObj), tmdesc::ref_obj<int>>::value, "");

    const int* constPtr = ptr;
    auto refConstObj    = tmdesc::inderect_ref(constPtr);
    static_assert(std::is_same<decltype(refConstObj), tmdesc::ref_obj<const int>>::value, "");

    CHECK(refConstObj.get() == v);
}
