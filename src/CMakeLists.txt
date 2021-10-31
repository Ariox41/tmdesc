add_library(tmdesc INTERFACE)
target_include_directories(tmdesc INTERFACE $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/>)

if(NOT CMAKE_VERSION VERSION_LESS 3.8)
    target_compile_features(tmdesc INTERFACE cxx_std_14)
endif()

add_library(tmdesc::tmdesc ALIAS tmdesc)

if(DEFINED TMDESC_IS_MAIN_PROJECT)
    if(TMDESC_IS_MAIN_PROJECT)
        # IDE development target (works in QtCreator an msvs)
        file(GLOB_RECURSE TM_DESK_ALL_HEADERS "*.hpp")
        add_custom_target(${PROJECT_NAME}_IDE SOURCES ${TM_DESK_ALL_HEADERS})
    endif()
endif()
