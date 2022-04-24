// Copyright Victor Smirnov 2021-2022
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
//
// The documentation can be found at the library's page:
// https://github.com/Ariox41/tmdesc

#pragma once
namespace tmdesc {
/** Attribute of type or type member.

    @tparam Key
    Tag for value search. Key defines the attribute type.

    @tparam T
    Type of attribute value

    @warning To find the attribute value, you only need to know the Key type.
    The `attribute<Key, T>` is only needed to describe the type, later it is converted to other structures.
 */
template <class Key, class T> struct attribute { T value; };

namespace tags {
/// Tag for typename attribute.
/// The value ot attribute has type of @ref zstring_view
struct type_name {};
} // namespace tags

/** Type info builder interface

    @details usage:
``` c++
namespace my_xml_lib{
    namespace tags{
        struct xml_array_item_name{};
    }
    constexpr auto xml_array_item_name(tmdesc::zstring_view name){
        return tmdesc::attribute<tags::xml_array_item_name, tmdesc::zstring_view>{name};
    }
}

struct User{
    UUID id;
    std::string name;

    tempate<class Impl>
    friend constexpr auto tmdesc_info(tmdesc::info_builder<User, Impl> builder){
        return builder.build(                             //
            builder.attributes(builder.typename("User")), //
            builder.members(                              //
                builder.member("id", &User::Id),          //
                builder.member("name", &User::name),      //
            )                                             //
        );
    }
};
struct UserGroup{
    std::string name;
    std::vector<User> users;
    std::vector<std::string> tags;
};
tempate<class Impl>
constexpr auto tmdesc_info(tmdesc::info_builder<UserGroup, Impl> builder){
    return builder.type(                                                  //
        builder.attributes(builder.type_name("UserGroup")),                //
        builder.members(                                                  //
            builder.member("name", &User::name),                          //
            builder.member("users", &User::users),                        //
            builder.member("tags", &User::tags,                           //
                builder.attributes(my_xml_lib::xml_array_item_name("tag"))//
            )                                                             //
        )                                                                 //
    );
}

void test(){
    tmdesc::zstring_view xml_string = R"(
    <UserGroup name="some_group_name">
        <users>
            <User id="97354c1a-bd80-11ec-9d64-0242ac120002" name="some_user_name" />
        </users>
        <tags>
            <tag>some_tag<tag>
        </tags>
    </UserGroup>
    )"
    UserGroup group = my_xml_lib::from_xml_string<UserGroup>(xml_string);
    assert(xml_string == my_xml_lib::to_prettified_xml_string(group));
}
```

    @note
    The type description has almost no effect on compilation time.
    Using this description increases compilation time, but only for those translation units that use the description.

    @note
    Specialization by default defines only the general interface. In reality, other specializations will be used with
    different types of arguments and return values, but with the same interface.
*/
template <class T, class Impl> class info_builder {
public:
    struct unspecified;
    template <class U> struct attribute_set;
    template <class U> struct member_info;
    template <class U> struct member_set_info;
    template <class U> struct type_info;

    /// wrap typename string to attribute
    constexpr attribute<tags::type_name, const char*> type_name(const char* name) const;

    /// wraps attributes to attribute_set
    template <class... Keys, class... Values>
    constexpr attribute_set<unspecified> attributes(attribute<Keys, Values>... attributes) const;

    /// wraps information about a member
    /// @param name - member name
    /// @param member - pointer to member of target type or pointer to member of target base type
    template <class M, class U> constexpr member_info<unspecified> member(const char* name, M T::*member) const;

    /// wraps information about a member
    /// @param name - member name
    /// @param member - pointer to member of target type or pointer to member of target base type
    /// @param attributes_ - member attributes, the result of the `attributes` function.
    template <class M, class U, class AS>
    constexpr member_info<unspecified> member(const char* name, M U::*member, attribute_set<AS> attributes_) const;

    template <class M, class U, class... Args> constexpr void member(const char*, M (T::*)(Args...)) const = delete;

    template <class M, class U, class... Args, class AS>
    constexpr void member(const char*, M (T::*)(Args...), attribute_set<AS> attributes_) const = delete;

    /// wraps information about member set to single struct
    template <class... U> constexpr member_set_info<unspecified> members(member_info<U>... members_) const;

    /// wraps information about type set to single struct
    /// @param member_set_ - type members info,  the result of the `members` function
    template <class M> constexpr type_info<unspecified> type(member_set_info<M> member_set_) const;

    /// wraps information about type set to single struct
    /// @param attributes_ - type attributes, the result of the `attributes` function.
    template <class AS> constexpr type_info<unspecified> type(attribute_set<AS> attributes_) const;

    /// wraps information about type set to single struct
    /// @param member_set_ - type members info,  the result of the `members` function
    /// @param attributes_ - type attributes, the result of the `attributes` function.
    template <class AS, class M>
    constexpr type_info<unspecified> type(attribute_set<AS> attributes_, member_set_info<M> member_set_) const;
};

} // namespace tmdesc
