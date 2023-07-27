// Copyright Victor Smirnov 2021-2022
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
//
// The documentation can be found at the library's page:
// https://github.com/Ariox41/tmdesc

#pragma once
#include <boost/hana/chain.hpp>
#include <boost/hana/contains.hpp>
#include <boost/hana/find.hpp>
#include <boost/hana/fold_right.hpp>
#include <boost/hana/functional/partial.hpp>
#include <boost/hana/integral_constant.hpp>
#include <boost/hana/lazy.hpp>
#include <boost/hana/optional.hpp>
#include <boost/hana/transform.hpp>
#include <boost/hana/union.hpp>
#include <tmdesc/config.hpp>
#include <tmdesc/type_info/detail/info_builder.hpp>
namespace tmdesc {
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
TMDESC_INLINE_VARIABLE constexpr get_type_info_fn get_type_info{};

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
TMDESC_INLINE_VARIABLE constexpr get_type_attributes_fn get_type_attributes{};

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
TMDESC_INLINE_VARIABLE constexpr get_type_members_info_fn get_type_members_info{};

struct find_type_attribute_fn {
    template <class Tag> struct helper {
        template <class AS> constexpr auto operator()(const AS& attributes) {
            return hana::find(attributes, hana::type_c<Tag>);
        }
    };
    /**
     * @brief returns the type attribute, if it exists
     * @param type - target type
     * @param attribute_tag - attribute tag
     * @return an optional attribute value
     */
    template <class T, class AttrTag>
    constexpr auto operator()(hana::basic_type<T> type, hana::basic_type<AttrTag> attribute_tag) const {
        (void)attribute_tag;
        return hana::chain(get_type_attributes(type), helper<AttrTag>{});
    }
};

/**
 * @brief A functional object that returns the type attribute, if it exists
 * @param type - target type
 * @param attribute_tag - attribute tag
 * @return an optional attribute value
 */
TMDESC_INLINE_VARIABLE constexpr find_type_attribute_fn find_type_attribute{};

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
TMDESC_INLINE_VARIABLE constexpr get_strong_type_info_fn get_strong_type_info{};

struct get_strong_type_members_info_fn {
    template <class T> constexpr const auto& operator()(hana::basic_type<T> type) const;
};
TMDESC_INLINE_VARIABLE constexpr get_strong_type_members_info_fn get_strong_type_members_info{};

struct get_strong_type_attributes_fn {
    template <class T> constexpr const auto& operator()(hana::basic_type<T> type) const;
};
TMDESC_INLINE_VARIABLE constexpr get_strong_type_attributes_fn get_strong_type_attributes{};

namespace detail {
template <class T> constexpr auto is_type_info(const T&) { return hana::false_c; }
template <class T, class AC, class AS> constexpr auto is_type_info(const tmdesc::type_info<T, AC, AS>&) {
    return hana::true_c;
}

template <typename T, typename = void> struct get_strong_type_info_impl {
    static constexpr const auto& apply() { return boost::hana::nothing; }
};

template <class T>
struct get_strong_type_info_impl<T, std::void_t<decltype(tmdesc_info(tmdesc::info_builder<T, tmdesc::_default>{}))>> {
    // Call free function `tmdesc_info(info_builder<T, unspecified>{})`, and return the result.
    // see [Argument-dependent lookup](https://en.cppreference.com/w/cpp/language/adl)
    // The arguments of the info_builder template contain the desired type, which means that
    // if a free function is defined in the type namespace, it will be called correctly.
    static constexpr auto apply() {
        constexpr auto info = tmdesc_info(tmdesc::info_builder<T, ::tmdesc::_default>{});
        static_assert(detail::is_type_info(info), "");
        return boost::hana::just(std::move(info));
    }
};

template <class T> TMDESC_INLINE_VARIABLE constexpr auto strong_type_info_cache = get_strong_type_info_impl<T>::apply();

template <class T> TMDESC_INLINE_VARIABLE constexpr auto strong_type_info_or_error = strong_type_info_cache<T>.value();

struct get_members_helper {
    template <typename T, typename MS, typename AS>
    constexpr auto operator()(const type_info<T, MS, AS>& type_info) const {
        return type_info.members();
    }
};
struct get_attributes_helper {
    template <typename T, typename MS, typename AS>
    constexpr auto operator()(const type_info<T, MS, AS>& type_info) const {
        return type_info.attributes();
    }
};
template <class T>
TMDESC_INLINE_VARIABLE constexpr auto strong_type_members_cache = hana::transform(strong_type_info_cache<T>,
                                                                                  get_members_helper{});

template <class T>
TMDESC_INLINE_VARIABLE constexpr auto strong_type_attributes_cache = hana::transform(strong_type_info_cache<T>,
                                                                                     get_attributes_helper{});

template <class T>
TMDESC_INLINE_VARIABLE constexpr auto inherited_types_or_error =
    hana::find(strong_type_info_cache<T>.value().attributes(), hana::type_c<attributes::inherits_tag>);

// template <class T>
// TMDESC_INLINE_VARIABLE constexpr auto need_flaten_transparent =
//    hana::contains(strong_type_info_cache<T>.value().attributes(), hana::type_c<attributes::flatten_transparent_tag>);

struct inherit_attributes_fn {
    template <class T> struct helper {
        template <class... Bs> constexpr auto operator()(attributes::inherits_tag::inherited_types_list<Bs...>) const {
            auto strong_attributes = strong_type_info_or_error<T>.attributes();
            //auto clean_strong_attributes = hana::erase_key(strong_attributes, hana::type_c<attributes::inherits_tag>);
            auto basic_attributes = hana::make_basic_tuple(strong_type_info_or_error<Bs>.attributes()...);
            return hana::fold_right(basic_attributes, strong_attributes, hana::union_);
        }
    };

    template <class T, class MS, class AS> constexpr auto operator()(const type_info<T, MS, AS>& type_info) const {
        return hana::transform(inherited_types_or_error<T>, helper<T>{}).value_or(type_info.attributes());
    }
};

template <class T>
TMDESC_INLINE_VARIABLE constexpr auto type_attributes_cache = hana::transform(strong_type_info_cache<T>,
                                                                              inherit_attributes_fn{});

// template <class T>
//TMDESC_INLINE_VARIABLE constexpr auto get_type_members_info_or_error = get_type_members_info(hana::type_c<T>).value();

struct flat_members_fn {
#ifndef TMDESC_CONFIG_DISABLE_FLATTEN_TRANSPARENT
    template <class T> struct flat_transparent_on_each {
        struct on_each_member_of_member {
            template <class OAC, class AC, class AS>
            constexpr auto operator()(const OAC& owner_accessor, const tmdesc::member_info<AC, AS>& member) const {
                auto accessor = hana::compose(hana::partial(tmdesc::invoke, member.accessor()),
                                              hana::partial(tmdesc::invoke, owner_accessor));
                return member_info<decltype(accessor), AS>{member.name(), accessor, member.attributes()};
            }
        };
        struct for_each_member_of_member {
            template <class OAC, class Members>
            constexpr auto operator()(const OAC& owner_accessor, const Members& members) const {
                return hana::transform(members, hana::curry<2>(on_each_member_of_member{})(owner_accessor));
            }
        };

        struct get_members_of_member {
            template <class AC, class AS> constexpr auto operator()(const tmdesc::member_info<AC, AS>& member) const {
                using MemberType = std::decay_t<tmdesc::invoke_result_t<AC, T>>;
                return hana::transform(get_type_members_info(hana::type_c<MemberType>),
                                       hana::curry<2>(for_each_member_of_member{})(member.accessor()))
                    .value_or(hana::make_basic_tuple());
            }
        };

        template <class AC, class AS> constexpr auto operator()(const tmdesc::member_info<AC, AS>& member) const {
            return hana::eval_if(hana::contains(member.attributes(), hana::type_c<attributes::transparent_tag>), //
                                 hana::make_lazy(get_members_of_member{})(member),
                                 hana::make_lazy(hana::make_basic_tuple)(member));
        }
    };
    struct flat_transparent_helper {
        template <class MS, class T> constexpr auto operator()(const MS& members, hana::basic_type<T>) const {
            return hana::chain(members, flat_transparent_on_each<T>{});
        }
    };
    struct first_arg {
        template <class T, class... Ts> constexpr decltype(auto) operator()(T&& first, Ts&&...) const {
            return static_cast<T&&>(first);
        }
    };

#endif
    template <class IM> struct inherit_helper_for {
        IM inherited_members;
        template <class MS> constexpr auto operator()(const MS& members) const {
            return hana::fold_right(inherited_members, members, hana::concat);
        }
    };
    struct inherit_helper {
        template <class... Bs> constexpr auto operator()(attributes::inherits_tag::inherited_types_list<Bs...>) const {
            constexpr auto inherited_members = hana::make_basic_tuple(strong_type_info_or_error<Bs>.members()...);
            return inherit_helper_for<decltype(inherited_members)>{inherited_members};
        }
    };

    template <class T, class MS, class AS> constexpr auto operator()(const type_info<T, MS, AS>& type_info) const {
#ifndef TMDESC_CONFIG_DISABLE_FLATTEN_TRANSPARENT
        auto members =
            hana::if_(hana::contains(type_info.attributes(), hana::type_c<attributes::flatten_transparent_tag>),
                      flat_transparent_helper{}, first_arg{})(type_info.members(), hana::type_c<T>);
#else
        auto members = type_info.members();
#endif

        return hana::maybe(hana::id, inherit_helper{}, inherited_types_or_error<T>)(members);
    }
};

template <class T>
TMDESC_INLINE_VARIABLE constexpr auto type_members_cache = hana::transform(strong_type_info_cache<T>,
                                                                           flat_members_fn{});

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
TMDESC_INLINE_VARIABLE constexpr auto type_info_cache = hana::transform(strong_type_info_cache<T>, inherit_info_fn{});

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

template <class T> constexpr const auto& get_strong_type_members_info_fn::operator()(hana::basic_type<T> type) const {
    (void)type;
    return tmdesc::detail::strong_type_members_cache<T>;
}

template <class T> constexpr const auto& get_strong_type_attributes_fn::operator()(hana::basic_type<T> type) const {
    (void)type;
    return tmdesc::detail::strong_type_attributes_cache<T>;
}

} // namespace tmdesc
