#pragma once
#include "static_type_info.hpp"
namespace tmdesc {
template <class T>
struct tuple_size<T, std::enable_if_t<has_type_info_v<T>>>
  : public tuple_size<decltype(static_members_info_v<meta::remove_cvref_t<T>>)> {};

namespace detail {
struct getter_for_struct_t {
    template <std::size_t I, class T> static constexpr decltype(auto) get_by_id(T&& s) noexcept {
        constexpr auto member_getter = get<I>(static_members_info_v<meta::remove_cvref_t<T>>);
        return member_getter.get(std::forward<T>(s));
    }

    /// \todo not implemented
    template <class T, class Tuple>
    static constexpr decltype(auto) get_by_type(Tuple&&) noexcept = delete;

    template <std::size_t I, class T>
    using tuple_element =
        typename tuple_element_t<I, decltype(static_members_info_v<T>)>::member_type;
};
} // namespace detail

template <class T> struct tuple_getter<T, std::enable_if_t<has_type_info_v<T>>> {
    using type = detail::getter_for_struct_t;
};

// template <std::size_t I, class Tuple,
// std::enable_if_t<has_type_info_v<meta::remove_cvref_t<Tuple>>, bool> = true> constexpr
// decltype(auto) get(Tuple&& t) noexcept {
//    return detail::getter_for_struct_t::template get_by_id<I>(std::forward<Tuple>(t));
//}

} // namespace tmdesc
