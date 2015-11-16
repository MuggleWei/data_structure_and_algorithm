cmake_minimum_required(VERSION 2.8.9)
project(data_structure_and_algorithm_c)

# source and head file of data structure
file(GLOB DATA_STRUCTURE_C_SOURCE src/*.c) 
file(GLOB DATA_STRUCTURE_C_HEADER inc/*.h) 

# add libary
set(data_structure_c_lib data_structure_c)
if (WIN32)
add_library(${data_structure_c_lib}
	${DATA_STRUCTURE_C_SOURCE}
	${DATA_STRUCTURE_C_HEADER}
)
else()
add_library(${data_structure_c_lib}
	${DATA_STRUCTURE_C_SOURCE}
)
endif()

# test code
if (BUILD_RUN_TEST)
    add_subdirectory(run_test)
endif()