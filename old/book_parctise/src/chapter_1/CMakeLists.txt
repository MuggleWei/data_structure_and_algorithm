
add_book_code(p1_1)
add_book_code(p1_2)
add_book_code(p1_4)

# set folder in vs
set_target_properties(
	p1_1 p1_2 p1_4
	PROPERTIES
	FOLDER "book/chapter1"
)