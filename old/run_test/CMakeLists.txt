cmake_minimum_required(VERSION 2.8.9)

# c array
file(GLOB RUN_TEST_C_ARRAY_SOURCE c_array/*.cpp)
file(GLOB RUN_TEST_C_ARRAY_HEADER c_array/*.h)
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
# end c array

# cpp array
file(GLOB RUN_TEST_CPP_ARRAY_SOURCE cpp_array/*.cpp)
file(GLOB RUN_TEST_CPP_ARRAY_HEADER cpp_array/*.h)
if (WIN32)
add_executable(run_test_cpp_array
	${RUN_TEST_CPP_ARRAY_SOURCE}
	${RUN_TEST_CPP_ARRAY_HEADER}
)
else()
add_executable(run_test_cpp_array
	${RUN_TEST_CPP_ARRAY_SOURCE}
)
endif()
target_link_libraries(run_test_cpp_array
	${utility_lib}
)
# end cpp array

# list
file(GLOB RUN_TEST_C_LIST_SOURCE c_list/*.cpp)
file(GLOB RUN_TEST_C_LIST_HEADER c_list/*.h)
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

# cpp list
file(GLOB RUN_TEST_CPP_LIST_SOURCE cpp_list/*.cpp)
file(GLOB RUN_TEST_CPP_LIST_HEADER cpp_list/*.h)
if (WIN32)
add_executable(run_test_cpp_list
	${RUN_TEST_CPP_LIST_SOURCE}
	${RUN_TEST_CPP_LIST_HEADER}
)
else()
add_executable(run_test_cpp_list
	${RUN_TEST_CPP_LIST_SOURCE}
)
endif()
target_link_libraries(run_test_cpp_list
	${utility_lib}
)
# end cpp list

# double list
file(GLOB RUN_TEST_C_DOUBLE_LIST_SOURCE c_double_list/*.cpp)
file(GLOB RUN_TEST_C_DOUBLE_LIST_HEADER c_double_list/*.h)
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

# cpp double list
file(GLOB RUN_TEST_CPP_DOUBLE_LIST_SOURCE cpp_double_list/*.cpp)
file(GLOB RUN_TEST_CPP_DOUBLE_LIST_HEADER cpp_double_list/*.h)
if (WIN32)
add_executable(run_test_cpp_double_list
	${RUN_TEST_CPP_DOUBLE_LIST_SOURCE}
	${RUN_TEST_CPP_DOUBLE_LIST_HEADER}
)
else()
add_executable(run_test_cpp_double_list
	${RUN_TEST_CPP_DOUBLE_LIST_SOURCE}
)
endif()
target_link_libraries(run_test_cpp_double_list	
	${utility_lib}
)
# end cpp list

# tree
file(GLOB RUN_TEST_C_TREE_SOURCE c_tree/*.cpp)
file(GLOB RUN_TEST_C_TREE_HEADER c_tree/*.h)
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

# tree
file(GLOB RUN_TEST_CPP_TREE_SOURCE cpp_tree/*.cpp)
file(GLOB RUN_TEST_CPP_TREE_HEADER cpp_tree/*.h)
if (WIN32)
add_executable(run_test_cpp_tree
	${RUN_TEST_CPP_TREE_SOURCE}
	${RUN_TEST_CPP_TREE_HEADER}
)
else()
add_executable(run_test_cpp_tree
	${RUN_TEST_CPP_TREE_SOURCE}
)
endif()
target_link_libraries(run_test_cpp_tree
	${utility_lib}
)
# end tree

# set folder in vs
set_target_properties(
	run_test_c_array run_test_c_list run_test_c_double_list run_test_c_tree
	run_test_cpp_array run_test_cpp_list run_test_cpp_double_list run_test_cpp_tree
	PROPERTIES
	FOLDER "RunTest"
)
install(TARGETS 
	run_test_c_array run_test_c_list run_test_c_double_list run_test_c_tree
	run_test_cpp_array run_test_cpp_list run_test_cpp_double_list run_test_cpp_tree
	RUNTIME DESTINATION ${DSAA_INSTALL_RUNTIME_PATH}
	LIBRARY DESTINATION ${DSAA_INSTALL_LIBRARY_PATH}
	ARCHIVE DESTINATION ${DSAA_INSTALL_ARCHIVE_PATH}
)