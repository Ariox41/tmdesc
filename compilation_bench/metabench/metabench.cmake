# Copyright Victor Smirnov 2021
# Distributed under the Boost Software License, Version 1.0.
# (See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
#
# This file is part of the simple compile-time benchmark tools for the tmdesc library. 
# The original idea is taken from https://github.com/ldionne/metabench

find_package(Ruby 2.4 QUIET)
if(NOT RUBY_EXECUTABLE)
    message(WARNING "Ruby >= 2.4 was not found; the metabench.cmake module can't be used.")
    return()
endif()

set(TMDESC_METABENCH_SELF_FILE_DIR ${CMAKE_CURRENT_LIST_DIR} CACHE INTERNAL "")

# Global target for bench all
add_custom_target(TMDESC_MATABENCH_ALL)


# Register dataset
#
# target - name for new cmake executable target, generated from erb_template_src.
#          target_link_library is supported.
#
# erb_template_src - the ERB templite file for c++ file generation.
#                    @item variable available in the erb template.
#                    The time is calculated as the difference between compiling code with defined METABENCH macro and without it

# dataset_generator - ruby expression for input array generation.
#                     The @item variable in erb template is an input array item.
# [NAME] - dataset readable name. Default: ${target}
# [REPETITIONS] - repetitions count for each item. Default: 1
function (tmdesc_metabench_add_dataset target erb_template_src dataset_generator)
    set(options)
    set(one_value_args NAME REPETITIONS)
    set(multi_value_args )
    cmake_parse_arguments(ARGS "${options}" "${one_value_args}" "${multi_value_args}" ${ARGN})

    if (NOT ARGS_NAME)
        set(ARGS_NAME ${target})
    endif()
    if (NOT ARGS_REPETITIONS)
        set(ARGS_REPETITIONS 1)
    endif()

    file(MAKE_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/metabench/")
    file(MAKE_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/metabench/${target}/")

    set(measure_result_path "${CMAKE_CURRENT_BINARY_DIR}/metabench/${target}/data.json")
    get_filename_component(erb_template_full_path "${erb_template_src}"
                           REALPATH BASE_DIR "${CMAKE_CURRENT_SOURCE_DIR}")

    # C++ file for get compilation command implementation
    set(measured_cpp_pattern_file_path "${CMAKE_CURRENT_BINARY_DIR}/metabench/${target}/measured.cpp.tmdesc_metabench_pattern.cpp")
    set(compilation_command_line_file_path "${CMAKE_CURRENT_BINARY_DIR}/metabench/${target}/compilation_command.txt")

    # Real file to measure the compilation time
    set(measured_cpp_file_path "${CMAKE_CURRENT_BINARY_DIR}/metabench/${target}/measured.cpp")

    # Source files
    set(ruby_bench_impl_src "${TMDESC_METABENCH_SELF_FILE_DIR}/metabench.rb")
    set(ruby_compile_impl_src "${CMAKE_CURRENT_BINARY_DIR}/metabench/${target}/metabench_compile.rb")

    file(GENERATE OUTPUT "${ruby_compile_impl_src}" CONTENT "                         \n\
        require 'benchmark'                                                           \n\
        require 'open3'                                                               \n\
        require 'fileutils'                                                           \n\
                                                                                      \n\
        command_line = ARGV.join(' ')                                                 \n\
                                                                                      \n\
        stdout, stderr, status = Open3.capture3(command_line)                         \n\
                                                                                      \n\
        real_command_line = command_line.gsub('.tmdesc_metabench_pattern.cpp', \"\")  \n\
                                                                                      \n\
        IO.write('${compilation_command_line_file_path}', \"#{real_command_line}\")   \n\
                                                                                      \n\
        $stdout.puts(stdout)                                                          \n\
        $stderr.puts(stderr)                                                          \n\
        exit(status.success?)                                                         \n\
        ")

    # pattern file to get compilation command.
    # Ninja does not allow calling 'cmake --build' in cmake custom_command, which is why this hack is used
    file(WRITE ${measured_cpp_pattern_file_path} "int main(){return 0;}")

    # target to get compilation command. Target dependency included in the compilation command.
    add_library(${target} OBJECT  ${measured_cpp_pattern_file_path})
    set_target_properties(${target} PROPERTIES
        RULE_LAUNCH_COMPILE "${RUBY_EXECUTABLE} -- \"${ruby_compile_impl_src}\""
        EXCLUDE_FROM_DEFAULT_BUILD TRUE
        EXCLUDE_FROM_ALL TRUE
        )

    # time measure target
    add_custom_command(OUTPUT "${measure_result_path}" COMMAND ${RUBY_EXECUTABLE}
        -r json
        -r fileutils
        -r ${ruby_bench_impl_src}
        -e "out = measure('${target}', '${dataset_generator}',  '${erb_template_full_path}', '${measured_cpp_file_path}', ${ARGS_REPETITIONS}, '$<TARGET_OBJECTS:${target}>', '${compilation_command_line_file_path}')"
        -e "out['name'] = '${ARGS_NAME}'"
        -e "IO.write('${measure_result_path}', JSON.pretty_generate(out))"
        SOURCES ${erb_template_full_path}
        DEPENDS ${erb_template_full_path} ${target}
        WORKING_DIRECTORY ${PROJECT_BINARY_DIR}
        VERBATIM USES_TERMINAL)

    # alias for time measure target, and target for IDE prject tree view
    add_custom_target("_metabench_.${target}"
        DEPENDS "${measure_result_path}"
        SOURCES ${erb_template_full_path})
    set_target_properties("_metabench_.${target}" PROPERTIES TMDESC_METABENCH_RESULT "${measure_result_path}")
endfunction()

# Add compilation time chart as html file in build dirrectory
# target - cmake target name
# DATASETS dataset1 [dataset2 [dataset3 [...]]] - list of dataset targets
# [ALL] - append to default target ALL
# [TITLE title] - chart title, default:  ${target}
# [XAXIS xaxis] - chart x axis title, default: "Input value"
# [YAXIS yaxis] - chart y axis title, default: "Time, s"
function(tmdesc_metabench_add_chart target)
    set(options ALL)
    set(one_value_args TITLE XAXIS YAXIS)
    set(multi_value_args DATASETS)
    cmake_parse_arguments(ARGS "${options}" "${one_value_args}" "${multi_value_args}" ${ARGN})

    if(NOT ARGS_TITLE)
        set(ARGS_TITLE ${target})
    endif()
    if(NOT ARGS_XAXIS)
        set(ARGS_XAXIS "Input value")
    endif()
    if(NOT ARGS_YAXIS)
        set(ARGS_YAXIS "Time, s")
    endif()

    set(html_erb_template_path "${TMDESC_METABENCH_SELF_FILE_DIR}/metabench_chart.html.erb")
    set(output_path "${CMAKE_CURRENT_BINARY_DIR}/${target}.html")

    set(datasets_targets )
    set(json_data_sets )
    foreach(dataset ${ARGS_DATASETS})
        list(APPEND datasets_targets "_metabench_.${dataset}")
        get_target_property(json_file_path "_metabench_.${dataset}" TMDESC_METABENCH_RESULT)
        list(APPEND json_data_sets ${json_file_path})
    endforeach()

    add_custom_command(
        OUTPUT "${output_path}"
        COMMAND "${RUBY_EXECUTABLE}" -r erb -r json -r fileutils
            -e "title = '${ARGS_TITLE}'"
            -e "x_axis = '${ARGS_XAXIS}'"
            -e "y_axis = '${ARGS_YAXIS}'"
            -e "data = '${json_data_sets}'.split(';').map { |path| IO.read(path) }"
            -e "html = ERB.new(File.read('${html_erb_template_path}')).result(binding)"
            -e "FileUtils.mkdir_p(File.dirname('${output_path}'))"
            -e "IO.write('${output_path}', html)"
        DEPENDS ${datasets_targets} ${json_data_sets} "${html_erb_template_path}"
        VERBATIM
    )
    if(ARGS_ALL)
        add_custom_target(${target} ALL DEPENDS "${output_path}")
    else()
        add_custom_target(${target} DEPENDS "${output_path}")
    endif()

    add_dependencies(TMDESC_MATABENCH_ALL ${target})
endfunction()
