option(BUILD_CHAPTER_1 "Build chapter 1" ON)
option(BUILD_CHAPTER_2 "Build chapter 2" ON)
option(BUILD_CHAPTER_3 "Build chapter 3" ON)

if (BUILD_CHAPTER_1)
add_subdirectory(chapter_1)
endif()

if (BUILD_CHAPTER_2)
add_subdirectory(chapter_2)
endif()

if (BUILD_CHAPTER_3)
add_subdirectory(chapter_3)
endif()