cmake_minimum_required (VERSION 3.0.2)
project(data_structure_and_algorithm)

# set compile parameter
if (${CMAKE_CXX_COMPILER_ID} STREQUAL GNU)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -march=native -Wall -Wextra -std=c++11")
elseif (${CMAKE_CXX_COMPILER_ID} MATCHES Clang)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -march=native -Wall -Wextra -Wno-missing-field-initializers")
elseif (${CMAKE_CXX_COMPILER_ID} STREQUAL MSVC)
    add_definitions(-D_CRT_SECURE_NO_WARNINGS=1 -D_UNICODE -DUNICODE)
endif()

# for vim plugin - YCM
if (NOT ${CMAKE_CXX_COMPILER_ID} STREQUAL MSVC)
	set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
endif()

# set output directory
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# set use folder in vs
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

# cmake
include(${CMAKE_CURRENT_LIST_DIR}/thirdparty/muggleCC/cmake/muggle_utils.cmake)

if (WIN32)
	list(APPEND CMAKE_PREFIX_PATH "f:/c_pkg")
else()
	list(APPEND CMAKE_PREFIX_PATH "~/c_pkg")
endif()


# muggleCC
add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/thirdparty/muggleCC)
set(mugglecc mugglecc)

# include directories
include_directories(
	${CMAKE_CURRENT_LIST_DIR}/src/
	${CMAKE_CURRENT_LIST_DIR}/thirdparty/muggleCC
)

# data structure lib
set(dsaa dsaa)
muggle_add_project(${dsaa} ${CMAKE_CURRENT_LIST_DIR}/src/dsaa SHARED)
add_dependencies(${dsaa} ${mugglecc})
target_link_libraries(${dsaa} PUBLIC ${mugglecc})

# functions
function(add_example name folder)
	message("add example ${name} ${folder}")
	
	set(name example_${name})

	file(GLOB tmp_h ${folder}/*.h)
	file(GLOB tmp_c ${folder}/*.c)
	file(GLOB tmp_cpp ${folder}/*.cpp)
	file(GLOB tmp_cc ${folder}/*.cc)

	if (WIN32)
		add_executable(${name} ${tmp_h} ${tmp_c} ${tmp_cpp} ${tmp_cc})
		set_target_properties(${name}
			PROPERTIES
			FOLDER "example"
			VS_DEBUGGER_WORKING_DIRECTORY "$(OutDir)"
		)
	else()
		add_executable(${name} ${tmp_c} ${tmp_cpp} ${tmp_cc})
	endif(WIN32)
	target_include_directories(${name} PUBLIC
		${CMAKE_CURRENT_LIST_DIR}/src
	)
	target_link_libraries(${name} ${dsaa})
	add_dependencies(${name} ${dsaa})
endfunction()

function(add_gtest name folder)
	message("add test ${name} ${folder}")
	
	set(name test_${name})

	file(GLOB tmp_h ${folder}/*.h)
	file(GLOB tmp_c ${folder}/*.c)
	file(GLOB tmp_cpp ${folder}/*.cpp)
	file(GLOB tmp_cc ${folder}/*.cc)

	if (WIN32)
		add_executable(${name} ${tmp_h} ${tmp_c} ${tmp_cpp} ${tmp_cc})
		set_target_properties(${name}
			PROPERTIES
			FOLDER "test"
			VS_DEBUGGER_WORKING_DIRECTORY "$(OutDir)"
		)
	else()
		add_executable(${name} ${tmp_c} ${tmp_cpp} ${tmp_cc})
	endif(WIN32)
	
	if (FOUND_GTEST_FROM_DOWNLOAD)
		add_dependencies(${name}
			${dsaa}
			${GTEST_BOTH_LIBRARIES}
		)
	endif()

	target_include_directories(${name} PRIVATE
		${GTEST_INCLUDE_DIRS}
	)
	target_link_libraries(${name}
		${dsaa}
		${GTEST_BOTH_LIBRARIES}
	)

	add_test(NAME ${name} COMMAND ${name})
endfunction()

# test
if (BUILD_TESTING)
	enable_testing()
	
	# search gtest first
	find_package(GTest)
	
	if (GTEST_FOUND)
		set(FOUND_GTEST_FROM_SEARCH ON)

		message("-- Find GTest - use gtest from search")
		message("-- GTest include directories: ${GTEST_INCLUDE_DIRS}")
		message("-- GTest both libraries: ${GTEST_BOTH_LIBRARIES}")
	else()

		set(GTEST_INCLUDE_DIRS "${gtest_SOURCE_DIR}/include")
		set(GTEST_BOTH_LIBRARIES gtest_main)
		set(FOUND_GTEST_FROM_DOWNLOAD ON)

		message("-- Find GTest - use gtest from download")
	endif()
endif()

# example
SUBDIRLIST(example_dirs ${CMAKE_CURRENT_LIST_DIR}/example)
FOREACH(subdir ${example_dirs})
	add_example(${subdir} ${CMAKE_CURRENT_LIST_DIR}/example/${subdir})
ENDFOREACH()

# test
if (BUILD_TESTING)
	SUBDIRLIST(test_root_dir ${CMAKE_CURRENT_LIST_DIR}/test)
	FOREACH(subdir ${test_root_dir})
		add_gtest(${subdir} ${CMAKE_CURRENT_LIST_DIR}/test/${subdir})
	ENDFOREACH()
endif()
