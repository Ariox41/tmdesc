file(GLOB TESTS_SRC "*.cpp")

add_executable(all_tests ${TESTS_SRC} doctest.h)
target_link_libraries(all_tests PRIVATE tmdesc)

set_property(TARGET all_tests PROPERTY CXX_STANDARD 14)

target_compile_options(all_tests PRIVATE
  $<$<CXX_COMPILER_ID:MSVC>:/W3 /WX>
  $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-Wall -Wextra -Wpedantic -Werror>
)
