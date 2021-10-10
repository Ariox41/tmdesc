#set(target_name tuple_foreach)

set(input_array_expr "(1..4).map{|x| x.pow(4) + 0}")
set(repeat_count 3)

tmdesc_metabench_add_dataset(simple_index_seq simple_index_seq.cpp.erb
    "${input_array_expr}" REPETITIONS ${repeat_count} NAME "simple, index_sequence")
target_link_libraries(simple_index_seq PRIVATE tmdesc::tmdesc)

tmdesc_metabench_add_dataset(simple_index_seq_with_invoke simple_index_seq_with_invoke.cpp.erb
    "${input_array_expr}" REPETITIONS ${repeat_count} NAME "simple, index_sequence + invoke")
target_link_libraries(simple_index_seq_with_invoke PRIVATE tmdesc::tmdesc)

tmdesc_metabench_add_dataset(simple_invoke_and_save_get simple_invoke_and_save_get.cpp.erb
    "${input_array_expr}" REPETITIONS ${repeat_count} NAME "simple, invoke + save get")
target_link_libraries(simple_invoke_and_save_get PRIVATE tmdesc::tmdesc)

tmdesc_metabench_add_dataset(func_obj_index_seq func_obj_index_seq.cpp.erb
    "${input_array_expr}" REPETITIONS ${repeat_count} NAME "fn_obj, index_sequence")
target_link_libraries(func_obj_index_seq PRIVATE tmdesc::tmdesc)

tmdesc_metabench_add_dataset(func_obj_index_seq_with_invoke_result func_obj_index_seq_with_invoke_result.cpp.erb
    "${input_array_expr}" REPETITIONS ${repeat_count} NAME "fn_obj, index_sequence + is_noexcept_invokable")
target_link_libraries(func_obj_index_seq_with_invoke_result PRIVATE tmdesc::tmdesc)

tmdesc_metabench_add_dataset(func_obj_unpack func_obj_unpack.cpp.erb
    "${input_array_expr}" REPETITIONS ${repeat_count} NAME "fn_obj, unpack")
target_link_libraries(func_obj_unpack PRIVATE tmdesc::tmdesc)

tmdesc_metabench_add_dataset(func_obj_repeat_n func_obj_repeat_n.cpp.erb
    "${input_array_expr}" REPETITIONS ${repeat_count} NAME "fn_obj, repeat_n")
target_link_libraries(func_obj_repeat_n PRIVATE tmdesc::tmdesc)

       
tmdesc_metabench_add_chart(tuple_foreach_chart DATASETS
    simple_index_seq
    simple_index_seq_with_invoke
    simple_invoke_and_save_get
    func_obj_index_seq
    func_obj_index_seq_with_invoke_result
    func_obj_unpack
    func_obj_repeat_n
)
