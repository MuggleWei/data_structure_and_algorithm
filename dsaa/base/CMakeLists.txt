# base lib
if (WIN32)
	add_library(${base_lib} ${LIB_TYPE}
		${BASE_HEADER}
		${BASE_SOURCE}
	)
else()
	add_library(${base_lib} ${LIB_TYPE}
		${BASE_SOURCE}
	)
endif()	
set_target_properties(${base_lib} PROPERTIES 
	COMPILE_DEFINITIONS "${LIB_MACRO}"
	LINKER_LANGUAGE C)
