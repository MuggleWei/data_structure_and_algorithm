cmake_minimum_required(VERSION 2.8.9)
project(run_test_data_structure_and_algorithm_c)

# array
file(GLOB RUN_TEST_C_ARRAY_SOURCE array/*.cpp)
file(GLOB RUN_TEST_C_ARRAY_HEADER array/*.h)
if (WIN32)
add_executable(run_test_c_array
	${RUN_TEST_C_ARRAY_SOURCE}
	${RUN_TEST_C_ARRAY_HEADER}
)
else()
add_executable(run_test_c_array
	${RUN_TEST_C_ARRAY_SOURCE}
)
endif()
target_link_libraries(run_test_c_array 
	${data_structure_c_lib} 
	${run_test_lib}
)
# end array

# list
file(GLOB RUN_TEST_C_LIST_SOURCE list/*.cpp)
file(GLOB RUN_TEST_C_LIST_HEADER list/*.h)
if (WIN32)
add_executable(run_test_c_list
	${RUN_TEST_C_LIST_SOURCE}
	${RUN_TEST_C_LIST_HEADER}
)
else()
add_executable(run_test_c_list
	${RUN_TEST_C_LIST_SOURCE}
)
endif()
target_link_libraries(run_test_c_list 
	${data_structure_c_lib}
	${run_test_lib}
)
# end list

# double list
file(GLOB RUN_TEST_C_DOUBLE_LIST_SOURCE double_list/*.cpp)
file(GLOB RUN_TEST_C_DOUBLE_LIST_HEADER double_list/*.h)
if (WIN32)
add_executable(run_test_c_double_list
	${RUN_TEST_C_DOUBLE_LIST_SOURCE}
	${RUN_TEST_C_DOUBLE_LIST_HEADER}
)
else()
add_executable(run_test_c_double_list
	${RUN_TEST_C_DOUBLE_LIST_SOURCE}
)
endif()
target_link_libraries(run_test_c_double_list 
	${data_structure_c_lib}
	${run_test_lib}
)
# end double list