cmake_minimum_required(VERSION 3.20)

project(sandbox)
set(CMAKE_CXX_STANDARD 17)

add_executable(sandbox
  src/sandbox.hpp 
  src/sandbox.cpp
  src/camera-controller.hpp
  src/camera-controller.cpp
)

target_link_libraries(sandbox PRIVATE geg)