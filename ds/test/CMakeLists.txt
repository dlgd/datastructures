include_directories(${GTEST_INCLUDE_DIR})
include_directories(${AUTOCHECK_INCLUDE_DIR})

add_executable (union_find_test union_find_test.cpp)
target_link_libraries (union_find_test ds gtest_main)

add_test(union_find union_find_test
  "${CMAKE_CURRENT_LIST_DIR}/tinyUF.txt"
  "${CMAKE_CURRENT_LIST_DIR}/mediumUF.txt"
  "${CMAKE_CURRENT_LIST_DIR}/largeUF.txt")


add_executable (sort_test sort_test.cpp)
target_link_libraries (sort_test gtest_main)

add_test(sort sort_test)


