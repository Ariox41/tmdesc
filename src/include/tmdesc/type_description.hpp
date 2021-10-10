// Copyright Victor Smirnov 2021
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)

#pragma once

#include "meta/primitives.hpp"
#include "string_view.hpp"
#include <cstdint>

namespace tmdesc {
template <class T> struct type_t : public meta::type_t<T> {};
template <class Tag, class Flag> struct flag { Flag flag; };

template <class Impl> struct member_info_wrapper;
template <class... Flags> struct flag_map;

/// Type info builder interface
/// \details usage:
/// \code
/// namespace foo{
///    struct bar{ int id; std::vector<std::string> groups; };
///
///    template<class Impl>
///    constexpr auto tmdesc_info(tmdesc::type_t<bar>, tmdesc::info_builder<Impl> builder){
///        return builder(builder.member("id", &bar::id),                          //
///                       builder.member("groups", &bar::groups,                   //
///                                      serialization::xml::array_item("group"))  //< custom flag of member
///                       serialization::type_name("bar") //< custom flag of type
///        )
///    }
/// }
/// \endcode
/// \note The type description does not affect compile time as long as it is not used \todo bench
template <class Impl> class info_builder {
    Impl impl_;

public:
    /// wraps custom flags set of member or type
    template <class... Tags, class... Flags>
    constexpr flag_map<flag<Tags, Flags>...> flags(const flag<Tags, Flags>&... flags) {
        return impl_.flags(flags...);
    }

    /// wraps information about a member
    /// \param name - member name
    /// \param member - pointer to member of target type or pointer to member of target base type
    /// \return an object of the member_info_wrapper</* unspecified */>
    template <class M, class T, class... Flags>
    constexpr TMDESC_DOXYGEN_AUTO(member_info_wrapper</* unspecified */>)
        member(zstring_view name, M T::*member) const noexcept {
        return impl_.member(name, member);
    }

    /// wraps information about a member
    /// \param name - member name
    /// \param member - pointer to member of target type or pointer to member of target base type
    /// \param flags - custom flags with custom implementation
    /// \note flags implementations must be constexpr
    /// \return an object of the member_info_wrapper<implementation_detail>
    template <class M, class T, class... Flags>
    constexpr TMDESC_DOXYGEN_AUTO(member_info_wrapper</* unspecified */>)
        member(zstring_view name, M T::*member, const flag_map<Flags...>& member_flags) const noexcept {
        return impl_.member(name, member, member_flags);
    }

    /// build type information
    /// \param members - information about members
    template <class... Members>
    constexpr TMDESC_DOXYGEN_AUTO(/* unspecified */)
    operator()(const member_info_wrapper<Members>&... members) const noexcept {
        return impl_(members...);
    }

    /// build type information
    /// \param members - information about members
    /// \param type_flags - optional set of specific flags
    template <class... Members, class... Flags>
    constexpr TMDESC_DOXYGEN_AUTO(/* unspecified */)
    operator()(const member_info_wrapper<Members>&... members,
               const flag_map<Flags...>& type_flags = {}) const noexcept {
        return impl_(members..., type_flags);
    }
};

} // namespace tmdesc
