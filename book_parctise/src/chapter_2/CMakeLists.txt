# code 2.4.3
file(GLOB C2_4_3_H c2_4_3/*.h)
file(GLOB C2_4_3_C c2_4_3/*.c)
file(GLOB C2_4_3_CPP c2_4_3/*.cpp)
if (WIN32)
add_executable(book_c2_4_3
	${C2_4_3_H}
	${C2_4_3_C}
	${C2_4_3_CPP}
)
else()
add_executable(book_c2_4_3
	${C2_4_3_C}
	${C2_4_3_CPP}
)
endif()
target_link_libraries(book_c2_4_3
	${data_structure_c_lib}
	${utility_lib}
)
# end practis 2.4.3

# set folder in vs
set_target_properties(
	book_c2_4_3
	PROPERTIES
	FOLDER "book/chapter2"
)