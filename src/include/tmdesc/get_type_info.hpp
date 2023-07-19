// Copyright Victor Smirnov 2021-2022
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
//
// The documentation can be found at the library's page:
// https://github.com/Ariox41/tmdesc

#pragma once
#include <boost/hana/chain.hpp>
#include <boost/hana/find.hpp>
#include <boost/hana/fold_right.hpp>
#include <boost/hana/functional/partial.hpp>
#include <boost/hana/integral_constant.hpp>
#include <boost/hana/optional.hpp>
#include <boost/hana/transform.hpp>
#include <boost/hana/union.hpp>
#include <tmdesc/type_info/detail/info_builder.hpp>
namespace tmdesc {

struct get_strong_type_info_fn {
    /**
     * @brief returns information about the type in its original form
     * @param type: hana::type<T>
     * @return an optional value of type @ref type_info
     * @note The `inherits` attribute is not processed
     */
    template <class T> constexpr const auto& operator()(hana::basic_type<T> type) const;
};

/**
 * @brief A functional object that returns information about the type in its original form
 * @param type: hana::type<T>
 * @return an optional value of type @ref type_info
 * @note The `inherits` attribute is not processed
 * @details
 * If the `tmdesc_info(info_builder<T, unspecified>)` free function is implemented for type T,
 * then return `just(type_info<unspecified>{})`.
 * Otherwise, return `nothing`.
 */
inline constexpr get_strong_type_info_fn get_strong_type_info{};

struct get_type_info_fn {
    /**
     * @brief returns information about a type given inheritance
     * @param type: hana::type<T>
     * @return an optional value of type @ref type_info
     */
    template <class T> constexpr const auto& operator()(hana::basic_type<T> type) const;
};
/**
 * @brief A functional object that returns information about the type given inheritance
 * @param type: hana::type<T>
 * @return an optional value of type @ref type_info
 * @details
 * If the `tmdesc_info(info_builder<T, unspecified>)` free function is implemented for type T,
 * then return `just(type_info<unspecified>{})`.
 * Otherwise, return `nothing`.
 */
inline constexpr get_type_info_fn get_type_info{};

struct get_type_attributes_fn {
    /**
     * @brief returns type attributes given inheritance
     * @param type: hana::type<T>
     * @return an optional value of type hana::map
     */
    template <class T> constexpr const auto& operator()(hana::basic_type<T> type) const;
};
/**
 * @brief A functional object that returns type attributes given inheritance
 * @param type: hana::type<T>
 * @return an optional value of type hana::map
 */
inline constexpr get_type_attributes_fn get_type_attributes{};

struct get_type_members_info_fn {
    /**
     * @brief returns type members given inheritance
     * @param type: hana::type<T>
     * @return an optional value of type hana::map
     */
    template <class T> constexpr const auto& operator()(hana::basic_type<T> type) const;
};
/**
 * @brief A functional object that returns type members given inheritance
 * @param type: hana::type<T>
 * @return an optional value of type hana::map
 */
inline constexpr get_type_members_info_fn get_type_members_info{};

namespace detail {
template <class T> constexpr auto is_type_info(const T&) { return hana::false_c; }
template <class T, class AC, class AS> constexpr auto is_type_info(const tmdesc::type_info<T, AC, AS>&) {
    return hana::true_c;
}

template <typename T, typename = void> struct get_strong_type_info_impl {
    static constexpr const auto& apply() { return boost::hana::nothing; }
};
//template <typename T> struct get_strong_type_info_impl<const T> : get_strong_type_info_impl<T> {};
//template <typename T> struct get_strong_type_info_impl<T&> : get_strong_type_info_impl<T> {};
//template <typename T> struct get_strong_type_info_impl<T&&> : get_strong_type_info_impl<T> {};

template <class T>
struct get_strong_type_info_impl<T, std::void_t<decltype(tmdesc_info(tmdesc::info_builder<T, tmdesc::_default>{}))>> {
    // Call free function `tmdesc_info(info_builder<T, unspecified>{})`, and return the result.
    // see [Argument-dependent lookup](https://en.cppreference.com/w/cpp/language/adl)
    // The arguments of the info_builder template contain the desired type, which means that
    // if a free function is defined in the type namespace, it will be called correctly.
    static constexpr auto apply() {
        auto info = tmdesc_info(tmdesc::info_builder<T, ::tmdesc::_default>{});
        static_assert(detail::is_type_info(info));
        return boost::hana::just(std::move(info));
    }
};

template <class T> inline constexpr auto strong_type_info_cache = get_strong_type_info_impl<T>::apply();

template <class T> inline constexpr auto strong_type_info_or_error = get_type_info(hana::type_c<T>).value();

struct get_basic_types_fn {
    template <typename T, typename MS, typename AS>
    constexpr auto operator()(const type_info<T, MS, AS>& type_info) const {
        return hana::find(type_info.attributes(), hana::type_c<attributes::inherits_tag>);
    }
};

template <class T> inline constexpr auto basic_types_or_error = get_basic_types_fn{}(*strong_type_info_cache<T>);

struct ingerit_attributes_fn {

    template <class T> struct helper {
        template <class... Bs> constexpr auto operator()(attributes::inherits_tag::basic_types_list<Bs...>) const {
            auto strong_attributes       = strong_type_info_or_error<T>.attributes();
            auto clean_strong_attributes = hana::erase_key(strong_attributes, hana::type_c<attributes::inherits_tag>);
            auto basic_attributes        = hana::make_basic_tuple(strong_type_info_or_error<Bs>.attributes()...);
            return hana::fold_right(basic_attributes, clean_strong_attributes, hana::union_);
        }
    };

    template <class T, class MS, class AS> constexpr auto operator()(const type_info<T, MS, AS>& type_info) const {
        return hana::transform(basic_types_or_error<T>, helper<T>{}).value_or(type_info.attributes());
    }
};
template <class T>
inline constexpr auto type_attributes_cache = hana::transform(strong_type_info_cache<T>, ingerit_attributes_fn{});

struct inherit_members_fn {
    template <class T> struct helper {
        template <class... Bs> constexpr auto operator()(attributes::inherits_tag::basic_types_list<Bs...>) const {
            return hana::fold_right(hana::make_basic_tuple(strong_type_info_or_error<Bs>.members()...), //
                                    strong_type_info_or_error<T>,                                       //
                                    hana::concat                                                        //
            );
        }
    };

    template <class T, class MS, class AS> constexpr auto operator()(const type_info<T, MS, AS>& type_info) const {
        return hana::transform(basic_types_or_error<T>, helper<T>{}).value_or(type_info.members());
    }
};

template <class T>
inline constexpr auto type_members_cache = hana::transform(strong_type_info_cache<T>, inherit_members_fn{});

struct inherit_info_fn {
    template <class T, class MS, class AS> static constexpr type_info<T, MS, AS> make_type_info(MS ms, AS as) {
        return {ms, as};
    }

    template <class TI> constexpr auto operator()(TI) const {
        using T = typename TI::type;
        return inherit_info_fn::make_type_info<T>(*type_members_cache<T>, *type_attributes_cache<T>);
    }
};

template <class T>
inline constexpr auto type_info_cache = hana::transform(strong_type_info_cache<T>, inherit_info_fn{});

// struct merge_base_class_info_helper {
//    template <class... Bs> static constexpr auto get_basic_members(attributes::inherits_tag::basic_types_list<Bs...>) {
//        return hana::make_basic_tuple(get_strong_type_info_or_error<Bs>.members()...);
//    }
//    template <class... Bs>
//    static constexpr auto get_basic_attributes(attributes::inherits_tag::basic_types_list<Bs...>) {
//        return hana::make_basic_tuple(get_strong_type_info_or_error<Bs>.attributes()...);
//    }

//    template <class T, class MS, class AS> constexpr auto operator()(const type_info<T, AS, MS>& base_info) const {
//        auto strong_types = hana::find(base_info.attributes(), hana::type_c<attributes::inherits_tag>);
//        if constexpr (strong_types == hana::nothing) {
//            return base_info;
//        } else {
//            auto members = hana::fold_right(get_basic_members(*strong_types), base_info.members(), hana::concat);

//            auto clean_attributes = hana::erase_key(base_info.attributes(), hana::type_c<attributes::inherits_tag>);
//            auto basic_attributes = get_basic_attributes(*strong_types);
//            // hana::union_ inserts from left to right
//            auto attributes = hana::fold_right(basic_attributes, clean_attributes, hana::union_);
//            return tmdesc::type_info<T, decltype(members), decltype(attributes)>{members, attributes};
//        }
//    }
//};

// template <class T>
//inline constexpr auto type_info_cache = hana::transform(strong_type_info_cache<T>, merge_base_class_info_helper{});
} // namespace detail

/**
 * @brief returns information about the type in its original form
 * @param type: hana::type<T>
 * @return an optional value of type @ref type_info
 */
template <class T> constexpr const auto& get_strong_type_info_fn::operator()(hana::basic_type<T> type) const {
    (void)type;
    return tmdesc::detail::strong_type_info_cache<T>;
}

template <class T> constexpr const auto& get_type_info_fn::operator()(hana::basic_type<T> type) const {
    (void)type;
    return tmdesc::detail::type_info_cache<T>;
}

template <class T> constexpr const auto& get_type_attributes_fn::operator()(hana::basic_type<T> type) const {
    (void)type;
    return tmdesc::detail::type_attributes_cache<T>;
}

template <class T> constexpr const auto& get_type_members_info_fn::operator()(hana::basic_type<T> type) const {
    (void)type;
    return tmdesc::detail::type_members_cache<T>;
}

} // namespace tmdesc
