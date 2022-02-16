cmake_minimum_required(VERSION 3.20)
project(geg-engine)

set(CMAKE_CXX_STANDARD 17)
add_library(sprivReflect STATIC 
  spirv_reflect.h
  spirv_reflect.c
)

target_include_directories(sprivReflect PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")
target_include_directories(sprivReflect PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/../unified1/")