// Copyright Victor Smirnov 2021
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)

#pragma once

#include "detail/default_info_builder.hpp"
#include "algorithm/transform.hpp"

namespace tmdesc {
struct struct_tie_t {
private:
    template <class StructRef> struct get_ref_visitor {
        StructRef&& ref;
        template <class MemberInfo> constexpr decltype(auto) operator()(const MemberInfo& mem_info) const noexcept {
            return mem_info.get_ref(ref);
        }
    };

public:
    template <class T, class Enable = std::enable_if_t<has_type_info_v<meta::remove_cvref_t<T>>>>
    constexpr auto operator()(T&& struct_) const noexcept {
        return tuple_transform(detail::type_info_v<meta::remove_cvref_t<T>>.members(), get_ref_visitor<T&&>{struct_});
    }
};
constexpr struct_tie_t struct_tie{};
} // namespace tmdesc
