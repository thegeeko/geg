SET(GCC_COVERAGE_COMPILE_FLAGS "-fPIC")
SET(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} ${GCC_COVERAGE_COMPILE_FLAGS}")

add_subdirectory(vendor/spdlog)
add_subdirectory(vendor/glfw)
add_subdirectory(vendor/imgui)
add_library(geg SHARED src/core/main-entry.hpp src/core/app.cpp src/core/app.hpp src/core/logger.cpp src/core/logger.hpp src/events/event.cpp src/events/event.hpp src/core/core.hpp src/window/window.cpp src/window/window.hpp src/events/event-types.hpp src/core/layer-stack.cpp src/core/layer.hpp src/core/layer-stack.hpp src/geg-pch.hpp src/glad/glad.h src/glad/glad.cpp src/imgui/im-gui.cpp src/imgui/im-gui.hpp src/core/input.cpp src/core/input.hpp src/core/key-codes.hpp src/renderer/buffers.hpp src/renderer/buffers.cpp src/renderer/renderer-api.cpp src/renderer/renderer-api.hpp src/backends/opengl/buffers.hpp src/backends/opengl/buffers.cpp src/renderer/vertex-array.cpp src/renderer/vertex-array.hpp src/backends/opengl/vertex-array.cpp src/backends/opengl/vertex-array.hpp)

target_link_libraries(geg PUBLIC spdlog)
target_link_libraries(geg PUBLIC glfw)
target_link_libraries(geg PUBLIC imgui)

target_include_directories(geg PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")
target_include_directories(geg PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/src")

target_precompile_headers(geg PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/src/geg-pch.hpp")