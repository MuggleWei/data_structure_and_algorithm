cmake_minimum_required (VERSION 2.8.9)

message("===== muggle dsaa start =====")

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
option(DSAA_SHARED "build shared library" ON)
option(DSAA_PERFORMANCE_OPTIMIZATION "optimize performance" ON)

# enable some opatimize for data structure performance
if (DSAA_PERFORMANCE_OPTIMIZATION)
	add_definitions(-DENABLE_DSAA_OPTIMIZATION)
endif()

# library type
if (DSAA_SHARED)
	set(LIB_TYPE SHARED)
	set(LIB_MACRO MG_USE_DLL)
else()
	set(LIB_TYPE STATIC)
	set(LIB_MACRO MG_USE_LIB)
	add_definitions(-DMG_USE_LIB)
endif()

# include
include_directories(
	c/inc
	cpp/inc
	base
	utility
)

# base files
file(GLOB BASE_HEADER ${CMAKE_CURRENT_SOURCE_DIR}/base/*.h)
file(GLOB BASE_SOURCE ${CMAKE_CURRENT_SOURCE_DIR}/base/*.c)

# utility files
if (DSAA_SHARED)
	file(GLOB UTILITY_HEADER ${CMAKE_CURRENT_SOURCE_DIR}/utility/*.h)
	file(GLOB UTILITY_SOURCE ${CMAKE_CURRENT_SOURCE_DIR}/utility/*.cpp)
else()
	file(GLOB UTILITY_SOURCE_ ${CMAKE_CURRENT_SOURCE_DIR}/utility/*.cpp)
	file(GLOB UTILITY_HEADER_ ${CMAKE_CURRENT_SOURCE_DIR}/utility/*.h)
	set (UTILITY_SOURCE
		${UTILITY_SOURCE_}
		${BASE_SOURCE}
	)
	set (UTILITY_HEADER
		${UTILITY_HEADER_}
		${BASE_HEADER}
	)
endif()

# libs
set(base_lib base)
set(data_structure_c_lib data_structure_c)
set(utility_lib utility)

# sub directory
add_subdirectory(base)
add_subdirectory(utility)
add_subdirectory(c)
add_subdirectory(cpp)

message("===== muggle dsaa end =====")