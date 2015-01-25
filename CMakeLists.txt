cmake_minimum_required(VERSION 3.1.0 FATAL_ERROR)
enable_testing()

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED on)

set(_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src/include)
include_directories(${_INCLUDE_DIR})

set(GTEST_DIR "gtest-1.7.0")
set(GTEST_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/${GTEST_DIR}/include)

add_subdirectory(${GTEST_DIR})
add_subdirectory(src)
add_subdirectory(test)