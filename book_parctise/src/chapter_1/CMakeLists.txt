# practis 1.1
file(GLOB P1_1_H p1_1/*.h)
file(GLOB P1_1_C p1_1/*.c)
file(GLOB P1_1_CPP p1_1/*.cpp)
if (WIN32)
add_executable(book_p1_1
	${P1_1_H}
	${P1_1_C}
	${P1_1_CPP}
)
else()
add_executable(book_p1_1
	${P1_1_C}
	${P1_1_CPP}
)
endif()
target_link_libraries(book_p1_1
	${data_structure_c_lib}
	${utility_lib}
)
# end practis 1.1

# practis 1.2
file(GLOB P1_2_H p1_2/*.h)
file(GLOB P1_2_C p1_2/*.c)
file(GLOB P1_2_CPP p1_2/*.cpp)
if (WIN32)
add_executable(book_p1_2
	${P1_2_H}
	${P1_2_C}
	${P1_2_CPP}
)
else()
add_executable(book_p1_2
	${P1_2_C}
	${P1_2_CPP}
)
endif()
target_link_libraries(book_p1_2
	${data_structure_c_lib}
	${utility_lib}
)
# end practis 1.2

# practis 1.4
file(GLOB P1_4_H p1_4/*.h)
file(GLOB P1_4_C p1_4/*.c)
file(GLOB P1_4_CPP p1_4/*.cpp)
if (WIN32)
add_executable(book_p1_4
	${P1_4_H}
	${P1_4_C}
	${P1_4_CPP}
)
else()
add_executable(book_p1_4
	${P1_4_C}
	${P1_4_CPP}
)
endif()
target_link_libraries(book_p1_4
	${data_structure_c_lib}
	${utility_lib}
)
# end practis 1.4