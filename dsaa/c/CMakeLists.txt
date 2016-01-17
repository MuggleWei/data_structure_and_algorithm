cmake_minimum_required(VERSION 2.8.9)

# source and head file of data structure
if (DSAA_SHARED)
	file(GLOB DATA_STRUCTURE_C_SOURCE src/*.c)
	file(GLOB DATA_STRUCTURE_C_HEADER inc/*.h)
else()
	file(GLOB DATA_STRUCTURE_C_SOURCE_ src/*.c)
	file(GLOB DATA_STRUCTURE_C_HEADER_ inc/*.h)
	set (DATA_STRUCTURE_C_SOURCE
		${DATA_STRUCTURE_C_SOURCE_}
		${BASE_SOURCE}
	)
	set (DATA_STRUCTURE_C_HEADER
		${DATA_STRUCTURE_C_HEADER_}
		${BASE_HEADER}
	)
endif()

# add libary
if (WIN32)
add_library(${data_structure_c_lib} ${LIB_TYPE}
	${DATA_STRUCTURE_C_SOURCE}
	${DATA_STRUCTURE_C_HEADER}
)
else()
add_library(${data_structure_c_lib} ${LIB_TYPE}
	${DATA_STRUCTURE_C_SOURCE}
)
endif()
set_target_properties(${data_structure_c_lib} PROPERTIES COMPILE_DEFINITIONS "${LIB_MACRO}")
if(DSAA_SHARED)
	target_link_libraries(${data_structure_c_lib} ${base_lib})
endif()