// Copyright Victor Smirnov 2021-2023
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
//
// The documentation can be found at the library's page:
// https://github.com/Ariox41/tmdesc
#include <tmdesc/string_view.hpp>
#include <boost/hana/optional.hpp>

namespace tmdesc {
/**
 * @brief Contains information about type T.
 * @tparam T - described type
 * @tparam MS - type members info, hana::tuple of member_info<...>
 * @tparam AS - type attributes, hana::map{type<Attribute> -> AttributeValue}.
 */
template <typename T, typename MS, typename AS> struct type_info {
    using type = T;

    constexpr type_info(MS members, AS attributes)
      : members_(std::move(members))
      , attributes_(std::move(attributes)) {}

    /**
     * @brief Returnsan optional tuple of @ref member_info objects.
     * @return `just(tuple(members...)))` or `nothing`.
     */
    constexpr const MS& members() const noexcept { return members_; }

    /**
     * @brief Returns an optional map{Tag->Value} of type attributes.
     * @return just(map{Tag->Value}) or `nothing`.
     */
    constexpr const AS& attributes() const noexcept { return attributes_; }

private:
    MS members_;
    AS attributes_;
};

} // namespace tmdesc
