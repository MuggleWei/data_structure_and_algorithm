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
	target_link_libraries(${base_lib}
		dl
	)
endif()	
set_target_properties(${base_lib} PROPERTIES 
	COMPILE_DEFINITIONS "${LIB_MACRO}"
	LINKER_LANGUAGE C)

# install
install(FILES ${BASE_HEADER} DESTINATION ${DSAA_INSTALL_INCLUDE_PATH}/base)
install(TARGETS ${base_lib}
	RUNTIME DESTINATION ${DSAA_INSTALL_RUNTIME_PATH}
	LIBRARY DESTINATION ${DSAA_INSTALL_LIBRARY_PATH}
	ARCHIVE DESTINATION ${DSAA_INSTALL_ARCHIVE_PATH}
)