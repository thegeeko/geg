cmake_minimum_required(VERSION 3.20)
project(geg-engine)

set(CMAKE_CXX_STANDARD 17)
set(GCC_COVERAGE_COMPILE_FLAGS "-fPIC")
set(CMAKE_CXX_FLAGS	"${CMAKE_CXX_FLAGS} ${GCC_COVERAGE_COMPILE_FLAGS}")

add_subdirectory(vendor/spdlog)
add_subdirectory(vendor/glfw)
add_subdirectory(vendor/glm)
add_subdirectory(vendor/imgui)
add_library(geg STATIC 
	src/geg-pch.hpp

	src/core/core.hpp
	src/core/core.cpp
	src/core/main-entry.hpp
	src/core/app.cpp
	src/core/app.hpp
	src/core/logger.cpp
	src/core/logger.hpp
	src/core/layer.hpp
	src/core/layer-stack.cpp
	src/core/layer-stack.hpp
	src/core/input.cpp
	src/core/input.hpp
	src/core/key-codes.hpp

	src/time/time.hpp
	src/time/time.cpp

	src/events/event.cpp
	src/events/event.hpp
	src/events/event-types.hpp

	src/window/window.cpp
	src/window/window.hpp

	src/glad/glad.h
	src/glad/glad.cpp

	src/imgui/im-gui.cpp
	src/imgui/im-gui.hpp

	src/renderer/cams/ortho-cam.hpp
	src/renderer/cams/ortho-cam.cpp
	
	src/renderer/buffers.hpp
	src/renderer/buffers.cpp
	src/renderer/shader.hpp
	src/renderer/shader.cpp
	src/renderer/renderer-api.cpp
	src/renderer/renderer-api.hpp
	src/renderer/renderer.cpp
	src/renderer/renderer.cpp
	src/renderer/vertex-array.cpp
	src/renderer/vertex-array.hpp
	src/renderer/renderer-commands.cpp
	src/renderer/renderer-commands.hpp
	src/renderer/graphics-context.cpp
	src/renderer/graphics-context.hpp

	src/backends/opengl/buffers.hpp
	src/backends/opengl/buffers.cpp
	src/backends/opengl/shader.hpp
	src/backends/opengl/shader.cpp
	src/backends/opengl/vertex-array.cpp
	src/backends/opengl/vertex-array.hpp
	src/backends/opengl/renderer-api.cpp
	src/backends/opengl/renderer-api.hpp
	src/backends/opengl/graphics-context.cpp
	src/backends/opengl/graphics-context.hpp

	src/backends/vulkan/graphics-context.cpp
	src/backends/vulkan/graphics-context.hpp
	src/backends/vulkan/vulkan-device.hpp
	src/backends/vulkan/vulkan-device.cpp
)

target_link_libraries(geg PUBLIC spdlog)
target_link_libraries(geg PUBLIC glfw)
target_link_libraries(geg PUBLIC imgui)
target_link_libraries(geg PUBLIC glm)
target_link_libraries(geg PUBLIC vulkan)

target_include_directories(geg PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")
target_include_directories(geg PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/src")

target_precompile_headers(geg PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/src/geg-pch.hpp")
if(MSVC)
	add_definitions(/FI"${CMAKE_CURRENT_SOURCE_DIR}/src/geg-pch.hpp")
else()
	# GCC or Clang
	add_definitions(-include "${CMAKE_CURRENT_SOURCE_DIR}/src/geg-pch.hpp")
endif()
