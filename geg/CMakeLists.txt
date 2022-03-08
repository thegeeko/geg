cmake_minimum_required(VERSION 3.20)
project(geg-engine)

set(CMAKE_CXX_STANDARD 17)
set(GCC_COVERAGE_COMPILE_FLAGS "-fPIC")
# set(MSVC_COVERAGE_COMPILE_FLAGS "/MP 12")

if(NOT MSVC)
	set(CMAKE_CXX_FLAGS	"${CMAKE_CXX_FLAGS} ${GCC_COVERAGE_COMPILE_FLAGS}")
else()
	# set(CMAKE_CXX_FLAGS	"${CMAKE_CXX_FLAGS} ${MSVC_COVERAGE_COMPILE_FLAGS}")
endif()


# ========= vulkan ==============
find_package(Vulkan REQUIRED)
# ==============================

# ============== glfw ==========================
if(WIN32)
	add_compile_definitions(_GLFW_WIN32=1)
elseif(UNIX AND NOT APPLE)
	add_compile_definitions(_GLFW_X11=1)
endif()


add_compile_definitions(_GLFW_VULKAN_STATIC)
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_VULKAN_STATIC ON CACHE BOOL "" FORCE)
add_subdirectory(vendor/glfw)
# ==============================================

# =============== assimp ============
set(ASSIMP_NO_EXPORT ON FORCE)
set(ASSIMP_BUILD_TESTS OFF FORCE)
add_subdirectory(vendor/assimp)
# ===================================

add_subdirectory(vendor/spdlog)
add_subdirectory(vendor/glm)
add_subdirectory(vendor/imgui)
add_subdirectory(vendor/vma)
add_subdirectory(vendor/spirv/reflect)

set(GEG_HEADERS 
	# core
	src/core/core.hpp
	src/core/app.hpp
	src/core/logger.hpp
	src/core/layer-stack.hpp
	src/core/input.hpp

	# engine
	src/time/time.hpp
	src/events/event.hpp
	src/window/window.hpp
	src/imgui/im-gui.hpp
	src/assets/loaders/loader.hpp
	src/assets/mesh.hpp

	# renderer
	src/renderer/cams/perspective.hpp
	src/renderer/buffers.hpp
	src/renderer/shader.hpp
	src/renderer/renderer-api.hpp
	src/renderer/renderer.hpp
	src/renderer/renderer.hpp
	src/renderer/pipeline.hpp
	src/renderer/graphics-context.hpp

	# renderer backend
	src/backends/vulkan/graphics-context.hpp
	src/backends/vulkan/vulkan-device.hpp
	src/backends/vulkan/swap-chain.hpp
	src/backends/vulkan/pipeline.hpp
	src/backends/vulkan/shader.hpp
	src/backends/vulkan/renderer-api.hpp
	src/backends/vulkan/vertex-buffer.hpp
	src/backends/vulkan/index-buffer.hpp
	src/backends/vulkan/descriptors.hpp
	src/backends/vulkan/uniform-buffers.hpp
)

add_library(geg STATIC 

	${GEG_HEADERS}

	# core
	src/core/core.cpp
	src/core/main-entry.cpp
	src/core/app.cpp
	src/core/logger.cpp
	src/core/layer-stack.cpp
	src/core/input.cpp

	# engine
	src/time/time.cpp
	src/events/event.cpp
	src/window/window.cpp
	src/imgui/im-gui.cpp
	src/assets/loaders/loader.cpp
	src/assets/mesh.cpp

	# renderer
	src/renderer/cams/perspective.cpp
	src/renderer/buffers.cpp
	src/renderer/shader.cpp
	src/renderer/renderer-api.cpp
	src/renderer/renderer.cpp
	src/renderer/renderer.cpp
	src/renderer/pipeline.cpp
	src/renderer/graphics-context.cpp

	# renderer backend
	src/backends/vulkan/graphics-context.cpp
	src/backends/vulkan/vulkan-device.cpp
	src/backends/vulkan/swap-chain.cpp
	src/backends/vulkan/pipeline.cpp
	src/backends/vulkan/shader.cpp
	src/backends/vulkan/renderer-api.cpp
	src/backends/vulkan/vertex-buffer.cpp
	src/backends/vulkan/index-buffer.cpp
	src/backends/vulkan/descriptors.cpp
	src/backends/vulkan/uniform-buffers.cpp
)

# =============== deps ======================
# vulkan .. vars should be defind by find vulkan or manuallay
target_link_libraries(geg PUBLIC Vulkan::Vulkan)

target_link_libraries(geg PUBLIC spdlog)
target_link_libraries(geg PUBLIC glfw)
target_link_libraries(geg PUBLIC imgui)
target_link_libraries(geg PUBLIC glm)
target_link_libraries(geg PUBLIC VulkanMemoryAllocator)
target_link_libraries(geg PUBLIC sprivReflect)
target_link_libraries(geg PUBLIC assimp)
# ==============================================

# =================== include dir ==============================
target_include_directories(geg PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")
target_include_directories(geg PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/src")
# ==============================================================

# ====================== geg pch =============================
# target_precompile_headers(geg PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/src/geg-pch.hpp")
if(MSVC)
	add_definitions(/FI"${CMAKE_CURRENT_SOURCE_DIR}/src/geg-pch.hpp")
else()
	# GCC or Clang
	add_definitions(-include "${CMAKE_CURRENT_SOURCE_DIR}/src/geg-pch.hpp")
endif()
# ==========================================================