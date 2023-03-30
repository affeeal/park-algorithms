cmake_minimum_required(VERSION 3.20)

project(park-algorithms)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -std=c++17 -Wall -Wextra -pedantic \
  -Wformat=2 -Wfloat-equal -Wconversion -Wlogical-op -Wshift-overflow=2 \
  -Wduplicated-cond -Wcast-qual -Wcast-align")

add_subdirectory(01)
add_subdirectory(02)
add_subdirectory(03)
add_subdirectory(04)
add_subdirectory(05)
add_subdirectory(06)

add_subdirectory(0a)
add_subdirectory(0b)
add_subdirectory(0c)
