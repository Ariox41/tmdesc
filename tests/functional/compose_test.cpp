#include <tmdesc/functional/compose.hpp>
#include "../test_helpers.hpp"
namespace test{
using tmdesc::compose;

inline constexpr auto f = [](int v){ return v + 1; };
inline constexpr auto g = [](int v){ return v + 10; };
inline constexpr auto h = [](int v){ return v * 10; };

STATIC_CHECK(compose(f, g, h)(42) == 42 * 10 + 10 + 1);

}
