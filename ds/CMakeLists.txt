project(datastructures)

set(TARGET ds)

set(_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)
set(_SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)

include_directories(${_INCLUDE_DIR})

set(PUB_HPP_FILES
  ${_INCLUDE_DIR}/ds/union_find.hpp
  ${_INCLUDE_DIR}/ds/sort.hpp
)

set(CPP_FILES
    ${_SRC_DIR}/union_find.cpp
)

add_library(${TARGET} SHARED ${CPP_FILES} ${HPP_FILES})
target_link_libraries(${TARGET})

add_subdirectory(test)