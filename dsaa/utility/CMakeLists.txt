cmake_minimum_required(VERSION 2.8.9)

# utility lib
if (WIN32)
	add_library(${utility_lib} ${LIB_TYPE}
		${UTILITY_SOURCE}
		${UTILITY_HEADER}
	)
else()
	add_library(${utility_lib} ${LIB_TYPE}
		${UTILITY_SOURCE}
	)
endif()
set_target_properties(${utility_lib} PROPERTIES 
	COMPILE_DEFINITIONS "${LIB_MACRO}"
	LINKER_LANGUAGE CXX)
if(DSAA_SHARED)
	target_link_libraries(${utility_lib} ${base_lib})
endif()