cmake_minimum_required(VERSION 3.14)
project(cube201)

set(CMAKE_CXX_STANDARD 14)

add_library(
        cube201 SHARED
        cubepos.h
        cubepos.cpp
)

add_executable(
  cubepos_test
  cubepos_test.cpp
)

add_executable(
  pair_search
  pair_search.cpp
)

target_link_libraries(
  cubepos_test
  cube201
)
target_link_libraries(
  pair_search
  cube201
)
