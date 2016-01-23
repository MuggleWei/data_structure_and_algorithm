option(BUILD_CHAPTER_1 "Build chapter 1" ON)

if (BUILD_CHAPTER_1)
add_subdirectory(chapter_1)
endif()