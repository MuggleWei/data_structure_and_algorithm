cmake_minimum_required (VERSION 2.8.9)

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