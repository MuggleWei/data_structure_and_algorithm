cmake_minimum_required(VERSION 2.8.9)

# source and head file of data structure
file(GLOB DATA_STRUCTURE_CPP_HEADER inc/*.h) 

# test code
if (DSAA_BUILD_RUN_TEST)
    add_subdirectory(run_test)
endif()