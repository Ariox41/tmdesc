#pragma once
#include <tmdesc/tmdesc_fwd.hpp>

namespace tmdesc {
namespace attrivutes {
constexpr attribute<name_t, zstring_view> name_t::operator()(zstring_view str) noexcept {
    return {str}
}
} // namespace attrivutes
} // namespace tmdesc
