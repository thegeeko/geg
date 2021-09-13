SET(GCC_COVERAGE_COMPILE_FLAGS "-fPIC")
SET(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} ${GCC_COVERAGE_COMPILE_FLAGS}")

add_subdirectory(vendor/spdlog)
add_subdirectory(vendor/glfw)
add_library(geg SHARED src/core/main-entry.hpp src/core/app.cpp src/core/app.hpp src/core/logger.cpp src/core/logger.hpp src/events/event.cpp src/events/event.hpp src/core/core.hpp src/window/window.cpp src/window/window.hpp src/events/event-types.hpp src/core/layer-stack.cpp src/core/layer.hpp src/core/layer-stack.hpp src/geg-pch.hpp)


target_link_libraries(geg PUBLIC spdlog)
target_link_libraries(geg PUBLIC glfw)
target_include_directories(geg PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")
target_include_directories(geg PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/src")
target_precompile_headers(geg PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/src/geg-pch.hpp")