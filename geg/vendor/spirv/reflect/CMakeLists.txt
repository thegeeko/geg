add_library(sprivReflect STATIC 
  spirv_reflect.h
  spirv_reflect.c
)

target_include_directories(sprivReflect PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")
target_include_directories(sprivReflect PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/../unified1/")