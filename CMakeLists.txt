cmake_minimum_required(VERSION 3.15)

project(park_algorithms)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -std=c++17 -Wall -Wextra -pedantic \
    -Wformat=2 -Wfloat-equal -Wconversion -Wlogical-op -Wshift-overflow=2 \
    -Wduplicated-cond -Wcast-qual -Wcast-align")

add_subdirectory(I)
add_subdirectory(II)
add_subdirectory(III)
