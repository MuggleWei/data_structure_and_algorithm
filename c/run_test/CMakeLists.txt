cmake_minimum_required(VERSION 2.8.9)

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
	${utility_lib}
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
	${utility_lib}
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
	${utility_lib}
)
# end double list

# tree
file(GLOB RUN_TEST_C_TREE_SOURCE tree/*.cpp)
file(GLOB RUN_TEST_C_TREE_HEADER tree/*.h)
if (WIN32)
add_executable(run_test_c_tree
	${RUN_TEST_C_TREE_SOURCE}
	${RUN_TEST_C_TREE_HEADER}
)
else()
add_executable(run_test_c_tree
	${RUN_TEST_C_TREE_SOURCE}
)
endif()
target_link_libraries(run_test_c_tree
	${data_structure_c_lib}
	${utility_lib}
)
# end tree