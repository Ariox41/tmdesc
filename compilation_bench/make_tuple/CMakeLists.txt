find_package(Boost 1.62)

set(input_array_expr "[0, 1, 2, 4, 16, 64, 256, 1024]")
set(repeat_count 3)

tmdesc_metabench_add_dataset(std_make_tuple std_make_tuple.cpp.erb
   "[0, 1, 2, 4, 8, 16, 32, 64]" REPETITIONS ${repeat_count} NAME "std::make_tuple")

tmdesc_metabench_add_dataset(tmdesc_make_tuple tmdesc_make_tuple.cpp.erb
    "${input_array_expr}" REPETITIONS ${repeat_count} NAME "tmdesc::make_tuple")
target_link_libraries(tmdesc_make_tuple PRIVATE tmdesc::tmdesc)

set(data_sets
    std_make_tuple
    tmdesc_make_tuple)
if(Boost_FOUND)
    tmdesc_metabench_add_dataset(hana_make_tuple hana_make_tuple.cpp.erb
    "${input_array_expr}" REPETITIONS ${repeat_count} NAME "boost::hana::make_tuple")
    target_link_libraries(hana_make_tuple PRIVATE Boost::boost)

    list(APPEND data_sets hana_make_tuple)
endif()

tmdesc_metabench_add_chart(make_tuple_chart DATASETS ${data_sets})
