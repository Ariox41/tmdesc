cmake_minimum_required (VERSION 3.7)
project (tmdesc VERSION  0.5 LANGUAGES CXX)

set(TMDESC_IS_MAIN_PROJECT OFF)
if(CMAKE_CURRENT_SOURCE_DIR STREQUAL CMAKE_SOURCE_DIR)
    set(TMDESC_IS_MAIN_PROJECT ON)
    message(INFO " tmdesc is main project. Compilation bench enabled")
    set(CXX_STANDARD 17)
endif()

add_subdirectory(src)

if(TMDESC_IS_MAIN_PROJECT)
    add_subdirectory(compilation_bench)

    add_subdirectory(examples)

    enable_testing() 
    add_subdirectory(tests)   

endif()
  


       
