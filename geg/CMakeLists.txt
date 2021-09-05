SET(GCC_COVERAGE_COMPILE_FLAGS "-fPIC")
SET(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} ${GCC_COVERAGE_COMPILE_FLAGS}")

add_subdirectory(vendor/spdlog)
add_library(geg SHARED src/main-entry.hpp src/app.cpp src/app.hpp src/logger.cpp src/logger.hpp)
target_link_libraries(geg PUBLIC spdlog)
target_include_directories(geg PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")
