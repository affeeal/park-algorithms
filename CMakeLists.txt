cmake_minimum_required(VERSION 3.15)

project(park_algorithms)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -std=c++17 -Wall -Wextra")

option(BUILD_TESTS "build tests")
if(BUILD_TESTS)
  enable_testing()
  find_package(GTest REQUIRED)
endif()

add_subdirectory(I)
add_subdirectory(II)
add_subdirectory(III)

