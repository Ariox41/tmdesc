find_package(doctest)
if(NOT doctest_FOUND)
  message(WARNING "doctest not found, testing is not possible")
  return()
endif()

set(TESTS_SRC
  main.cpp
  string_view_test.cpp
  tuple_test.cpp
  struct_algorithm.cpp
  struct_tie_test.cpp
)

add_executable(all_tests ${TESTS_SRC})
target_link_libraries(all_tests PRIVATE doctest::doctest tmdesc)

set_property(TARGET all_tests PROPERTY CXX_STANDARD 14)

target_compile_options(all_tests PRIVATE
  $<$<CXX_COMPILER_ID:MSVC>:/W3>
  $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-Wall -Wextra >
)
