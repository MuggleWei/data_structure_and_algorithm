cmake_minimum_required (VERSION 2.8.9)
project (DSAA)

# set cmake policy
# cmake_policy(SET CMP0054 NEW)

# set compile parameter
if (${CMAKE_CXX_COMPILER_ID} STREQUAL GNU)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -march=native -Wall -Wextra -std=c++11")
elseif (${CMAKE_CXX_COMPILER_ID} MATCHES Clang)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -march=native -Wall -Wextra -Wno-missing-field-initializers")
elseif (${CMAKE_CXX_COMPILER_ID} STREQUAL MSVC)
    add_definitions(-D_CRT_SECURE_NO_WARNINGS=1 -D_UNICODE -DUNICODE)
endif()

# set output directory
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# set use folder in vs
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

# ===== DSAA lib start =====
# add lib
add_subdirectory(dsaa)

# include directory
include_directories(
	dsaa/c/inc
	dsaa/cpp/inc
	dsaa/base
	dsaa/utility
)

# add macro
if (DSAA_PERFORMANCE_OPTIMIZATION)
	add_definitions(-DENABLE_DSAA_OPTIMIZATION)
endif()

# library type
if (DSAA_SHARED)
else()
	add_definitions(-DMG_USE_LIB)
endif()

# platform macro
if (${CMAKE_CXX_COMPILER_ID} STREQUAL GNU)
	add_definitions(-DMG_PLATFORM_UNIX=1)
elseif (${CMAKE_CXX_COMPILER_ID} MATCHES Clang)
	add_definitions(-DMG_PLATFORM_OSX=1)
elseif (${CMAKE_CXX_COMPILER_ID} STREQUAL MSVC)
	add_definitions(-DMG_PLATFORM_WINDOWS=1)
endif()

# set variable name
set(base_lib base)
set(data_structure_c_lib data_structure_c)
set(utility_lib utility)

# ===== DSAA lib end =====

option(DSAA_BUILD_RUN_TEST "Build test code" ON)
option(DSAA_BUILD_BOOK_PRACTISE "Build book practise" ON)

# build run test
if (DSAA_BUILD_RUN_TEST)
    add_subdirectory(run_test)
endif()

# build book practise
if (DSAA_BUILD_BOOK_PRACTISE)
	add_subdirectory(book_parctise)
endif()