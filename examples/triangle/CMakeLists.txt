cmake_minimum_required(VERSION 3.20)

project(triangle)
set(CMAKE_CXX_STANDARD 17)

add_executable(triangle src/sandbox.cpp src/sandbox.hpp)
target_link_libraries(triangle PRIVATE geg)
