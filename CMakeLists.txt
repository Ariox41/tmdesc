cmake_minimum_required (VERSION 3.7)
project (tmdesc VERSION  0.5 LANGUAGES CXX)



set(TMDESC_IS_MAIN_PROJECT OFF)
if(CMAKE_CURRENT_SOURCE_DIR STREQUAL CMAKE_SOURCE_DIR)
    set(TMDESC_IS_MAIN_PROJECT ON)
    #set(CXX_STANDART 17)
    message(INFO " tmdesc is main project. Compilation bench enabled")
endif()

find_package(Boost COMPONENTS headers REQUIRED)
add_subdirectory(src)
target_link_libraries(tmdesc INTERFACE Boost::headers)

if(TMDESC_IS_MAIN_PROJECT)
    add_subdirectory(compilation_bench)

    add_subdirectory(examples)

    enable_testing() 
    add_subdirectory(tests)   

endif()
  


       
