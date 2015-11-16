cmake_minimum_required(VERSION 2.8.9)
project(data_structure_and_algorithm_cpp)

# source and head file of data structure
file(GLOB DATA_STRUCTURE_CPP_HEADER inc/*.h) 

# test code
if (BUILD_RUN_TEST)
    add_subdirectory(run_test)
endif()