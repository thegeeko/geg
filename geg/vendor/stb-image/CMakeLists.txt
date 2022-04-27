add_library(stbimage STATIC 
  stb_image.h
  stb_image.cpp
)

target_include_directories(stbimage PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")