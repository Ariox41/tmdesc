// Copyright Victor Smirnov 2021-2022
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
//
// The documentation can be found at the library's page:
// https://github.com/Ariox41/tmdesc

#pragma once
#include <tmdesc/config.hpp>
namespace tmdesc {

class string_view;
class zstring_view;

/** Attribute of type or type member.

    @tparam Key
    Tag for value search. Key defines the attribute type.

    @tparam T
    Type of attribute value

    @warning To find the attribute value, you only need to know the Key type.
    The `attribute<Key, T>` is only needed to describe the type, later it is converted to other structures.
 */
template <typename Key, typename T> struct attribute { T value; };

namespace attributes {

struct type_name_tag {

    constexpr attribute<type_name_tag, const char*> operator()(const char* str) const noexcept { return {str}; }
    // #include <tmdesc/attributes.hpp>
    constexpr attribute<type_name_tag, zstring_view> operator()(zstring_view str) const noexcept;
};
TMDESC_INLINE_VARIABLE constexpr type_name_tag type_name{};

struct inherits_tag {
    template <typename...> struct inherited_types_list {};
};
/// Creates an attribute containing a list of inherited types
template <typename... Ts>
TMDESC_INLINE_VARIABLE constexpr attribute<inherits_tag, inherits_tag::inherited_types_list<Ts...>> inherits;

struct transparent_tag {
    constexpr attribute<transparent_tag, transparent_tag> operator()() const { return {}; }
};

/// Indicates the "transparency" of a member in the tree.
/// In a way, this is the equivalent of inheritance expressed through aggregation.
/// Unlike inheritance, transparency is not applied by default (it would be expensive),
/// you need to specify the `flatten_transparent` attribute for the type.
TMDESC_INLINE_VARIABLE constexpr transparent_tag transparent{};

struct flatten_transparent_tag {
    constexpr attribute<flatten_transparent_tag, flatten_transparent_tag> operator()() const { return {}; }
};
/// Indicates that transparent members should be flattened, similar to inheritance.
TMDESC_INLINE_VARIABLE constexpr flatten_transparent_tag flatten_transparent{};

} // namespace attributes

/** Type info builder interface
 *
 *  @details usage:
 *  ``` c++
 *  namespace my_xml_lib{
 *      struct array_item_tag_t{
 *          constexpr tmdesc::attribute<array_item_tag_t, zstring_view> operator()(zstring_view name) noexcept {
 *              return {name};
 *          }
 *      };
 *      inline constexpr array_item_tag_t array_item_tag{};
 *
 *      // ----- implementation -----
 *  }
 *
 *  using tmdesc::attributes::type_name;
 *  using tmdesc::attributes::inherits;
 *
 *  struct User{
 *      UUID id;
 *      std::string name;
 *
 *      tempate<typename Impl>
 *      friend constexpr auto tmdesc_info(tmdesc::info_builder<User, Impl> type){
 *          return type.build(                             //
 *              type.attributes(type_name("User")),        //
 *              type.members(                              //
 *                  type.member("Id", &User::Id),          //
 *                  type.member("Name", &User::name),      //
 *              )                                          //
 *          );
 *      }
 *  };
 *
 *  struct Employee: public User{
 *      std::string& activity;
 *
 *      tempate<typename Impl>
 *      friend constexpr auto tmdesc_info(tmdesc::info_builder<Employee, Impl> type){
 *          return type.build(                                                          //
 *                 type.attributes(type_name("Employee"), inherits<User>),            //
 *                 type.members(                                                        //
 *                     // Unable to create a pointer to a reference                     //
 *                     type.member("Activity", [](auto&& employee)->decltype(auto){     //
 *                         return std::forward<decltype(employee)>(employee).activity;  //
 *                     })                                                               //
 *                 )                                                                    //
 *          );
 *      }
 *  };
 *
 *  struct EmployeeGroup{
 *      std::string name;
 *      std::vector<std::string> activities;
 *      std::vector<Employee> employees;
 *  };
 *  tempate<typename Impl>
 *  constexpr auto tmdesc_info(tmdesc::info_builder<UserGroup, Impl> type){
 *      return type.build(                                                 //
 *          type.attributes(type_name("EmployeeGroup")),                   //
 *          type.members(                                                  //
 *              type.member("Name", &User::name),                          //
 *              type.member("Activities", &User::activities,               //
 *                  {my_xml_lib::array_item_tag("Activity")}               //
 *              ),                                                         //
 *              // The xml tag is a type name by default                   //
 *              type.member("Employees", &User::employees)                 //
 *          )                                                              //
 *      );
 *  }
 *
 *  void test(){
 *      tmdesc::zstring_view xml_string = R"(
 *      <EmployeeGroup Name="some_group_name">
 *          <Employees>
 *              <Employee
 *                  Id="97354c1a-bd80-11ec-9d64-0242ac120002"
 *                  Name="some_user_name"
 *                  Activity="some_activity"
 *              />
 *          </Employees>
 *          <Activities>
 *              <Activity>some_activity<Activity>
 *          </Activities>
 *      </EmployeeGroup>
 *      )"
 *      EmployeeGroup group = my_xml_lib::from_xml_string<UserGroup>(xml_string);
 *      assert(xml_string == my_xml_lib::to_prettified_xml_string(group));
 *  }
 *  ```
 *
 *  @note
 *  The type description has almost no effect on compilation time.
 *  Using this description increases compilation time, but only for those translation units that use the description.
 *
 *  @note
 *  Specialization by default defines only the general interface. Other specializations will be used with
 *  different types of arguments and return values, but with the same interface.
 */
template <typename T, typename Impl> class info_builder {
public:
    struct _unspecified;
    struct _attributes;
    struct _members;
    template <typename K> struct _main_attribute;

    template <typename U> struct _member_info;
    template <typename V> struct _attribute_set;

    /// wraps type attributes
    template <typename... Keys, typename... Values>
    constexpr _main_attribute<_attributes> attributes(const tmdesc::attribute<Keys, Values>&... attributes) const;

    /// wraps information about members
    template <typename... U> constexpr _main_attribute<_members> members(const _member_info<U>&... members_) const;

    /// wraps information about a member
    /// @param name - member name
    /// @param member - pointer to member of target type or pointer to member of target base type
    /// @param attributes - member attributes.
    template <typename M, typename U, typename... Keys, typename... Values>
    constexpr _member_info<_unspecified> member(const char* name, M U::*member,
                                                attribute<Keys, Values>... attributes) const;

    /// wraps information about a member
    /// @param name - member name
    /// @param accessor - functional object for member access (lvalue and rvalue)
    /// @param attributes - member attributes.
    template <typename M, typename Accessor, typename... Keys, typename... Values>
    constexpr _member_info<_unspecified> member(const char* name, Accessor accessor,
                                                attribute<Keys, Values>... attributes) const;

    /// wraps information about a member
    /// @param name - member name
    /// @param accessor - functional object for member access (lvalue and rvalue)
    /// @param attributes - member attributes from info_builder::attributes().
    template <typename M, typename Accessor>
    constexpr _member_info<_unspecified> member(const char* name, Accessor accessor,
                                                _main_attribute<_attributes>) const;

    /// wraps information about type
    template <typename... Ts> constexpr _unspecified build(_main_attribute<Ts>...) const;
};

} // namespace tmdesc
