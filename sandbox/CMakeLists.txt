cmake_minimum_required(VERSION 3.16)

project(sandbox)
set(CMAKE_CXX_STANDARD 17)

add_executable(sandbox
  src/sandbox.hpp 
  src/sandbox.cpp
  src/camera-controller.hpp
  src/camera-controller.cpp
)

target_link_libraries(sandbox PRIVATE geg)

add_custom_target(copy_assets
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/assets ${CMAKE_CURRENT_BINARY_DIR}/assets
)
add_dependencies(sandbox copy_assets)
