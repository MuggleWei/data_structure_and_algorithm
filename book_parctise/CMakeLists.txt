cmake_minimum_required (VERSION 2.8.9)

# function for add book practice code
function(add_book_code name)
	file(GLOB ${name}_H ${name}/*.h)
	file(GLOB ${name}_C ${name}/*.c)
	file(GLOB ${name}_CPP ${name}/*.cpp)
	if (WIN32)
	add_executable(${name}
		${${name}_H}
		${${name}_C}
		${${name}_CPP}
	)
	else()
	add_executable(${name}
		${${name}_C}
		${${name}_CPP}
	)
	endif()
	target_link_libraries(${name}
		${data_structure_c_lib}
		${utility_lib}
	)
endfunction(add_book_code) 

# add book code
add_subdirectory(src)

# add external command
if (${CMAKE_CXX_COMPILER_ID} STREQUAL MSVC)
set(custom_command "${CMAKE_CURRENT_SOURCE_DIR}/custom_command.cmd")
else()
set(custom_command "${CMAKE_CURRENT_SOURCE_DIR}/custom_command.sh")
endif()
add_custom_target(FAKE ALL
	# COMMAND ${CMAKE_COMMAND} -E remove_directory ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/res
	# COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIRECTORY}/res ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/res
	COMMAND ${custom_command} "res" ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
)