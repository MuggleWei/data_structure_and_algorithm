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

# set output directory
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# option for build test code
option(BUILD_RUN_TEST "Build test code" ON)

# include
include_directories(
	c/inc
	cpp/inc
	utility
)

# sub directory
add_subdirectory(c)
add_subdirectory(cpp)