SET(GCC_COVERAGE_COMPILE_FLAGS "-fPIC")
SET(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} ${GCC_COVERAGE_COMPILE_FLAGS}")

add_subdirectory(vendor/spdlog)
add_library(geg SHARED src/core/main-entry.hpp src/core/app.cpp src/core/app.hpp src/core/logger.cpp src/core/logger.hpp src/events/event.cpp src/events/event.hpp)
target_link_libraries(geg PUBLIC spdlog)
target_include_directories(geg PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")
