cmake_minimum_required(VERSION 2.8.9)
project(run_test_data_structure_and_algorithm_cpp)

# array
file(GLOB RUN_TEST_CPP_ARRAY_SOURCE array/*.cpp)
file(GLOB RUN_TEST_CPP_ARRAY_HEADER array/*.h)
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
	${run_test_lib}
)
# end array