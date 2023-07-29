add_subdirectory(to_json_idea)

add_executable(simple_usage simple_usage.cpp)
target_link_libraries(simple_usage PRIVATE tmdesc)

