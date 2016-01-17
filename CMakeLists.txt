cmake_minimum_required (VERSION 2.8.9)
project (DSAA)

# set cmake policy
# cmake_policy(SET CMP0054 NEW)

# set compile parameter
if (${CMAKE_CXX_COMPILER_ID} STREQUAL GNU)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -march=native -Wall -Wextra -Werror -std=c++11")
elseif (${CMAKE_CXX_COMPILER_ID} MATCHES Clang)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -march=native -Wall -Wextra -Werror -Wno-missing-field-initializers")
elseif (${CMAKE_CXX_COMPILER_ID} STREQUAL MSVC)
    add_definitions(-D_CRT_SECURE_NO_WARNINGS=1)
endif()

# set output directory
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# DSAA lib start
add_subdirectory(dsaa)
include_directories(
	dsaa/c/inc
	dsaa/cpp/inc
	dsaa/base
	dsaa/utility
)
if (DSAA_PERFORMANCE_OPTIMIZATION)
	add_definitions(-DENABLE_DSAA_OPTIMIZATION)
endif()
set(base_lib base)
set(data_structure_c_lib data_structure_c)
set(utility_lib utility)
# DSAA lib end

# build run test
option(DSAA_BUILD_RUN_TEST "Build test code" ON)
if (DSAA_BUILD_RUN_TEST)
    add_subdirectory(run_test)
endif()