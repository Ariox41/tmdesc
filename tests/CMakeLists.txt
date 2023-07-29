find_package(doctest)
if(NOT doctest_FOUND)
    message(WARNING "doctest not found, testing is not possible")
    return()
endif()


file(GLOB_RECURSE tmdesc_tests_src "**.cpp" "*.hpp")

## ================ c++17 ================

add_executable(tmdesc_tests ${tmdesc_tests_src})
target_link_libraries(tmdesc_tests PRIVATE doctest::doctest tmdesc::tmdesc)
set_target_properties(tmdesc_tests PROPERTIES
    CXX_STANDARD 17
)
target_compile_options(tmdesc_tests PRIVATE
    $<$<CXX_COMPILER_ID:MSVC>:/W3>
    $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-Wall -Wextra >
)

add_test(NAME tmdesc COMMAND tmdesc_tests)

## ================ c++14 ================

add_executable(tmdesc_tests_cpp14 ${tmdesc_tests_src})
target_link_libraries(tmdesc_tests_cpp14 PRIVATE doctest::doctest tmdesc::tmdesc)
set_target_properties(tmdesc_tests_cpp14 PROPERTIES
    CXX_STANDARD 14
)
target_compile_options(tmdesc_tests_cpp14 PRIVATE
    $<$<CXX_COMPILER_ID:MSVC>:/W3>
    $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-Wall -Wextra -Werror -pedantic >
)

add_test(NAME tmdesc_cpp14 COMMAND tmdesc_tests_cpp14)
