#pragma once
#include <tmdesc/string_view.hpp>
#include <tmdesc/tmdesc_fwd.hpp>

namespace tmdesc {
template <> struct attribute<attributes::type_name_tag, const char*> { zstring_view value; };
namespace attributes {
constexpr attribute<type_name_tag, zstring_view> type_name_tag::operator()(zstring_view str) const noexcept {
    return {str};
}
} // namespace attributes
} // namespace tmdesc
