find_package(doctest)
if(NOT doctest_FOUND)
  message(WARNING "doctest not found, testing is not possible")
  return()
endif()


file(GLOB_RECURSE tmdesc_tests_src "**.cpp")


add_executable(all_tests ${tmdesc_tests_src})
target_link_libraries(all_tests PRIVATE doctest::doctest tmdesc)

set_property(TARGET all_tests PROPERTY CXX_STANDARD 14)

target_compile_options(all_tests PRIVATE
  $<$<CXX_COMPILER_ID:MSVC>:/W3>
  $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-Wall -Wextra >
)

add_test(NAME tmdesc COMMAND all_tests)
