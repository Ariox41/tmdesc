#set(target_name tuple_foreach)

set(input_array_expr "(1..4).map{|x| x.pow(4) + 0}")
set(repeat_count 3)

tmdesc_metabench_add_dataset(simplest_index_seq simplest_index_seq.cpp.erb
    "${input_array_expr}" REPETITIONS ${repeat_count} NAME "simplest, index_sequence + noexcept(fn())")
target_link_libraries(simplest_index_seq PRIVATE tmdesc::tmdesc)

tmdesc_metabench_add_dataset(simplest_index_seq_with_invoke simplest_index_seq_with_invoke.cpp.erb
    "${input_array_expr}" REPETITIONS ${repeat_count} NAME "simplest, index_sequence + noexcept(invoke)")
target_link_libraries(simplest_index_seq_with_invoke PRIVATE tmdesc::tmdesc)


tmdesc_metabench_add_dataset(simplest_invoke_and_tuple_only_get simplest_invoke_and_tuple_only_get.cpp.erb
    "${input_array_expr}" REPETITIONS ${repeat_count} NAME "simplest, index_sequence + noexcept(invoke) + tuple only get")
target_link_libraries(simplest_invoke_and_tuple_only_get PRIVATE tmdesc::tmdesc)


tmdesc_metabench_add_dataset(tmdesc_get_index_seq tmdesc_get_index_seq.cpp.erb
    "${input_array_expr}" REPETITIONS ${repeat_count} NAME "tmdes::get: index_sequence + noexcept(invoke)")
target_link_libraries(tmdesc_get_index_seq PRIVATE tmdesc::tmdesc)

tmdesc_metabench_add_dataset(tmdesc_get_index_seq_invoke_result tmdesc_get_index_seq_invoke_result.cpp.erb
    "${input_array_expr}" REPETITIONS ${repeat_count} NAME "tmdes::get: index_sequence + is_nothrow_invokable")
target_link_libraries(tmdesc_get_index_seq_invoke_result PRIVATE tmdesc::tmdesc)

tmdesc_metabench_add_dataset(tmdesc_get_repeat_n tmdesc_get_repeat_n.cpp.erb
    "${input_array_expr}" REPETITIONS ${repeat_count} NAME "tmdes::get: repeat_n + noexcept(invoke)")
target_link_libraries(tmdesc_get_repeat_n PRIVATE tmdesc::tmdesc)

tmdesc_metabench_add_dataset(tmdesc_get_unpack tmdesc_get_unpack.cpp.erb
    "${input_array_expr}" REPETITIONS ${repeat_count} NAME "tmdes::get: unpack + noexcept(invoke)")
target_link_libraries(tmdesc_get_unpack PRIVATE tmdesc::tmdesc)
       
tmdesc_metabench_add_chart(tuple_foreach_chart DATASETS
    simplest_index_seq
    simplest_index_seq_with_invoke
    simplest_invoke_and_tuple_only_get
    tmdesc_get_index_seq
    tmdesc_get_index_seq_invoke_result
    tmdesc_get_repeat_n
    tmdesc_get_unpack
)
