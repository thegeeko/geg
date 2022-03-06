add_library(tinyObj INTERFACE)
target_sources(tinyObj INTERFACE tiny_obj_loader.h)
target_include_directories(tinyObj INTERFACE "${CMAKE_CURRENT_SOURCE_DIR}")