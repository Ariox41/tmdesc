#include "../test_helpers.hpp"
#include <tmdesc/functional/invoke.hpp>
#include <tmdesc/functional/ref_obj.hpp>

namespace nothrow_const {
struct Fn {
    int v;

    constexpr int operator()(int k) const noexcept { return v * k; }

    constexpr int mem_fn(int k) const noexcept { return 10 * v * k; }
};

constexpr int free_function(const Fn& fn, int k) noexcept { return 100 * fn.v * k; }

static_assert(tmdesc::is_invocable_v<const Fn&, int>, "");
static_assert(std::is_same<tmdesc::invoke_result_t<const Fn&, int>, int>::value, "");
static_assert(tmdesc::is_nothrow_invocable_v<const Fn&, int>, "");

static_assert(tmdesc::is_invocable_v<decltype(&Fn::mem_fn), const Fn&, int>, "");
static_assert(std::is_same<tmdesc::invoke_result_t<decltype(&Fn::mem_fn), const Fn&, int>, int>::value, "");

static_assert(tmdesc::is_invocable_v<decltype(&free_function), const Fn&, int>, "");
static_assert(std::is_same<tmdesc::invoke_result_t<decltype(&free_function), const Fn&, int>, int>::value, "");

static_assert(tmdesc::is_invocable_v<decltype(&Fn::v), const Fn&>, "");
static_assert(tmdesc::is_nothrow_invocable_v<decltype(&Fn::v), const Fn&>, "");
} // namespace nothrow_const

TEST_CASE("invoke nothrow const functions") {
    using namespace nothrow_const;
    constexpr const Fn fnObj{10};
    STATIC_CHECK(420 == tmdesc::invoke(fnObj, 42));
    STATIC_CHECK(4200 == tmdesc::invoke(&Fn::mem_fn, fnObj, 42));
    STATIC_CHECK(42000 == tmdesc::invoke(&free_function, fnObj, 42));

    STATIC_CHECK(10 == tmdesc::invoke(&Fn::v, fnObj));
}

namespace nothrow_mut {
struct Fn {
    int v;

    constexpr int operator()(int k) noexcept { return v * k; }

    constexpr int mem_fn(int k) noexcept { return 10 * v * k; }
};

constexpr int free_function(Fn& fn, int k) noexcept { return 100 * fn.v * k; }

static_assert(!tmdesc::is_invocable_v<const Fn&, int>, "");
static_assert(!tmdesc::is_nothrow_invocable_v<const Fn&, int>, "");

static_assert(tmdesc::is_invocable_v<Fn&, int>, "");
static_assert(tmdesc::is_nothrow_invocable_v<Fn&, int>, "");
static_assert(std::is_same<tmdesc::invoke_result_t<Fn&, int>, int>::value, "");

static_assert(!tmdesc::is_invocable_v<decltype(&Fn::mem_fn), const Fn&, int>, "");
static_assert(tmdesc::is_invocable_v<decltype(&Fn::mem_fn), Fn&, int>, "");
static_assert(std::is_same<tmdesc::invoke_result_t<decltype(&Fn::mem_fn), Fn&, int>, int>::value, "");

static_assert(!tmdesc::is_invocable_v<decltype(&free_function), const Fn&, int>, "");
static_assert(tmdesc::is_invocable_v<decltype(&free_function), Fn&, int>, "");
static_assert(std::is_same<tmdesc::invoke_result_t<decltype(&free_function), Fn&, int>, int>::value, "");

static_assert(tmdesc::is_invocable_v<decltype(&Fn::v), Fn&>, "");
static_assert(tmdesc::is_nothrow_invocable_v<decltype(&Fn::v), Fn&>, "");
} // namespace nothrow_mut

TEST_CASE("invoke nothrow mut functions") {
    using namespace nothrow_mut;
    Fn fnObj{10};
    CHECK(420 == tmdesc::invoke(fnObj, 42));
    CHECK(4200 == tmdesc::invoke(&Fn::mem_fn, fnObj, 42));
    CHECK(42000 == tmdesc::invoke(&free_function, fnObj, 42));

    CHECK(10 == tmdesc::invoke(&Fn::v, fnObj));
}

namespace nothrow_rvalue {
struct Fn {
    int v;

    constexpr int operator()(int k) && noexcept { return v * k; }

    constexpr int mem_fn(int k) && noexcept { return 10 * v * k; }
};

constexpr int free_function(Fn&& fn, int k) noexcept { return 100 * fn.v * k; }

static_assert(!tmdesc::is_invocable_v<const Fn&, int>, "");
static_assert(!tmdesc::is_nothrow_invocable_v<const Fn&, int>, "");
static_assert(!tmdesc::is_invocable_v<Fn&, int>, "");
static_assert(!tmdesc::is_nothrow_invocable_v<Fn&, int>, "");
static_assert(tmdesc::is_invocable_v<Fn&&, int>, "");
static_assert(tmdesc::is_nothrow_invocable_v<Fn&&, int>, "");
static_assert(std::is_same<tmdesc::invoke_result_t<Fn&&, int>, int>::value, "");

static_assert(!tmdesc::is_invocable_v<decltype(&Fn::mem_fn), const Fn&, int>, "");
static_assert(!tmdesc::is_invocable_v<decltype(&Fn::mem_fn), Fn&, int>, "");
static_assert(tmdesc::is_invocable_v<decltype(&Fn::mem_fn), Fn&&, int>, "");
static_assert(std::is_same<tmdesc::invoke_result_t<decltype(&Fn::mem_fn), Fn&&, int>, int>::value, "");

static_assert(!tmdesc::is_invocable_v<decltype(&free_function), const Fn&, int>, "");
static_assert(!tmdesc::is_invocable_v<decltype(&free_function), Fn&, int>, "");
static_assert(tmdesc::is_invocable_v<decltype(&free_function), Fn&&, int>, "");
static_assert(std::is_same<tmdesc::invoke_result_t<decltype(&free_function), Fn&&, int>, int>::value, "");

static_assert(tmdesc::is_invocable_v<decltype(&Fn::v), Fn&&>, "");
static_assert(tmdesc::is_nothrow_invocable_v<decltype(&Fn::v), Fn&&>, "");
} // namespace nothrow_rvalue

TEST_CASE("invoke nothrow rvalue functions") {
    using namespace nothrow_rvalue;

    CHECK(420 == tmdesc::invoke(Fn{10}, 42));
    CHECK(4200 == tmdesc::invoke(&Fn::mem_fn, Fn{10}, 42));
    CHECK(42000 == tmdesc::invoke(&free_function, Fn{10}, 42));

    CHECK(10 == tmdesc::invoke(&Fn::v, Fn{10}));
}

int get42() { return 42; }
int mul42(int v) { return v * 42; }

TEST_CASE("invoke ref_obj as function") {
    CHECK(tmdesc::invoke(tmdesc::ref(get42)) == 42);
    CHECK(tmdesc::invoke(tmdesc::ref(mul42), 10) == 420);
}

struct multi_arg_fn{
    int operator()(char v1, int v2, unsigned v3, unsigned char v4, int v5) const noexcept{
        return v1 + v2 + v3 +v4 +v5;
    }
};
inline constexpr multi_arg_fn multi_arg;
TEST_CASE("invoke ref_obj as function") {
    CHECK(tmdesc::invoke(multi_arg, 1, 2, 3, 4, 5) == 1 + 2 + 3 +  4 + 5);
}
