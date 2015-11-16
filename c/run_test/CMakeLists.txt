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
target_link_libraries(run_test_c_array ${data_structure_c_lib})