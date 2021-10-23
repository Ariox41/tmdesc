// Copyright Victor Smirnov 2021
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
//
// The documentation can be found at the library's page:
// https://github.com/Ariox41/tmdesc

#pragma once
#include "algorithm/transform.hpp"
#include "static_type_info.hpp"

namespace tmdesc {
struct struct_tie_t {
private:
    template <class StructRef> struct get_visitor {
        StructRef&& ref;
        template <class MemberInfo>
        constexpr decltype(auto) operator()(const MemberInfo& mem_info) const noexcept {
            return mem_info.get(ref);
        }
    };

public:
    template <class T, class Enable = std::enable_if_t<has_type_info_v<meta::remove_cvref_t<T>>>>
    constexpr auto operator()(T&& struct_) const noexcept {
        return transform_to<tuple>(static_members_info_v<meta::remove_cvref_t<T>>,
                                   get_visitor<T&&>{struct_});
    }
};
constexpr struct_tie_t struct_tie{};
} // namespace tmdesc
