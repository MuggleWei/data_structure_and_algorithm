cmake_minimum_required (VERSION 2.8.9)
project (data_structure_and_algoritm)

# set cmake policy
# cmake_policy(SET CMP0054 NEW)

# set compile parameter
if (${CMAKE_CXX_COMPILER_ID} STREQUAL GNU)
	message("using GCC")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -march=native -Wall -Wextra -Werror -std=c++11")
elseif (${CMAKE_CXX_COMPILER_ID} MATCHES Clang)
	message("using Clang")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -march=native -Wall -Wextra -Werror -Wno-missing-field-initializers")
elseif (${CMAKE_CXX_COMPILER_ID} STREQUAL MSVC)
	message("using Visual Studio")
    add_definitions(-D_CRT_SECURE_NO_WARNINGS=1)
endif()
message("CMAKE_BUILD_TYPE is ${CMAKE_BUILD_TYPE}")
message("CMAKE_CXX_FLAGS is ${CMAKE_CXX_FLAGS}")
message("CMAKE_CXX_FLAGS_DEBUG is ${CMAKE_CXX_FLAGS_DEBUG}")
message("CMAKE_CXX_FLAGS_RELEASE is ${CMAKE_CXX_FLAGS_RELEASE}")

# set output directory
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# option for build test code
option(BUILD_RUN_TEST "Build test code" ON)
option(DATA_STRUCTURE_PERFORMANCE_OPTIMIZATION "optimize data structure performance" ON)

# enable some opatimize for data structure performance
if (DATA_STRUCTURE_PERFORMANCE_OPTIMIZATION)
add_definitions(-DENABLE_DATA_STRUCTURE_OPTIMIZATION)
endif()

# include
include_directories(
	c/inc
	cpp/inc
	utility
)

# utility c file
set(utility_c_lib utility_c)
file(GLOB UTILITY_C_SOURCE ${CMAKE_CURRENT_SOURCE_DIR}/utility/*.c)
set(UTILITY_C_HEADER
	${CMAKE_CURRENT_SOURCE_DIR}/utility/base.h
	${CMAKE_CURRENT_SOURCE_DIR}/utility/macros.h
	${CMAKE_CURRENT_SOURCE_DIR}/utility/memory_pool.h
)
if (WIN32)
add_library(${utility_c_lib}
	${UTILITY_C_SOURCE}
	${UTILITY_C_HEADER}
)
else()
add_library(${utility_c_lib}
	${UTILITY_C_SOURCE}
)
endif()

# run test lib
set(run_test_lib run_test)
if (WIN32)
add_library(${run_test_lib}
	utility/performance_test.h
	utility/performance_test.cpp
)
else()
add_library(${run_test_lib}
	utility/performance_test.cpp
)
endif()

# sub directory
add_subdirectory(c)
add_subdirectory(cpp)