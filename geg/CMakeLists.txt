cmake_minimum_required(VERSION 3.16)
project(geg-engine)

# ========= setup ================
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake/modules")
set(GCC_COVERAGE_COMPILE_FLAGS "-fPIC")
set(MSVC_COVERAGE_COMPILE_FLAGS "")

if(NOT MSVC)
	set(CMAKE_CXX_FLAGS	"${CMAKE_CXX_FLAGS} ${GCC_COVERAGE_COMPILE_FLAGS}")
else()
	set(CMAKE_CXX_FLAGS	"${CMAKE_CXX_FLAGS} ${MSVC_COVERAGE_COMPILE_FLAGS}")
endif()
# ================================

# ========= deps ================
# vulkan loader
find_package(Vulkan REQUIRED)

# 3d models loading
#find_package(Assimp REQUIRED)

# shader compiliation
find_package(Shaderc REQUIRED)

# shader reflection
add_subdirectory(vendor/spirv/reflect)

# memroy allocator for vulkan
set(VMA_DEBUG_ALWAYS_DEDICATED_MEMORY ON CACHE BOOL "" FORCE)
set(VMA_DEBUG_INITIALIZE_ALLOCATIONS OFF CACHE BOOL "" FORCE)
add_subdirectory(vendor/vma)

# glfw
if(WIN32)
	add_compile_definitions(_GLFW_WIN32=1)
endif()

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_VULKAN_STATIC ON CACHE BOOL "" FORCE)
set(GLFW_USE_WAYLAND ON CACHE BOOL "" FORCE)
add_subdirectory(vendor/glfw)


add_subdirectory(vendor/spdlog)
add_subdirectory(vendor/glm)
add_subdirectory(vendor/imgui)
add_subdirectory(vendor/entt)
add_subdirectory(vendor/stb-image)
# ===================================


# =============== geg ===============
# find all source files and headers
file(
	GLOB_RECURSE # recursive
	GEG_SRC # variable to store the source files and headers
	CONFIGURE_DEPENDS # make a dependency
	"${PROJECT_SOURCE_DIR}/src/*.hpp" "${PROJECT_SOURCE_DIR}/src/*.cpp"
)
add_library(geg STATIC ${GEG_SRC})
# ===========================================

# =============== linking deps ======================
target_link_libraries(geg PUBLIC Vulkan::Vulkan)
target_link_libraries(geg PUBLIC shaderc)

target_link_libraries(geg PUBLIC spdlog)
target_link_libraries(geg PUBLIC glfw)
target_link_libraries(geg PUBLIC imgui)
target_link_libraries(geg PUBLIC glm)
target_link_libraries(geg PUBLIC VulkanMemoryAllocator)
target_link_libraries(geg PUBLIC sprivReflect)
target_link_libraries(geg PUBLIC assimp)
target_link_libraries(geg PUBLIC entt)
target_link_libraries(geg PUBLIC stbimage)
# ==============================================

# =================== include dir ==============================
target_include_directories(geg PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")
target_include_directories(geg PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/src")
# ==============================================================

# ====================== geg build =============================
# target_precompile_headers(geg PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/src/geg-pch.hpp")
if(MSVC)
	add_definitions("/FI ${CMAKE_CURRENT_SOURCE_DIR}/src/geg-pch.hpp")
	target_compile_options(geg PRIVATE "/MP 12")
else()
	# GCC or Clang
	add_definitions(-include "${CMAKE_CURRENT_SOURCE_DIR}/src/geg-pch.hpp")
endif()
# ==========================================================
