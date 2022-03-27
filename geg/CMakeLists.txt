cmake_minimum_required(VERSION 3.16)
project(geg-engine)

set(CMAKE_CXX_STANDARD 17)
set(GCC_COVERAGE_COMPILE_FLAGS "-fPIC")
# set(MSVC_COVERAGE_COMPILE_FLAGS "/MP 12")

if(NOT MSVC)
	set(CMAKE_CXX_FLAGS	"${CMAKE_CXX_FLAGS} ${GCC_COVERAGE_COMPILE_FLAGS}")
else()
	# set(CMAKE_CXX_FLAGS	"${CMAKE_CXX_FLAGS} ${MSVC_COVERAGE_COMPILE_FLAGS}")
endif()


# ========= graphics ==============
# vulkan loader
find_package(Vulkan REQUIRED)

# other vulkan libs / shaderc + spirvcross

if(DEFINED ENV{VULKAN_SDK})
	if (CMAKE_BUILD_TYPE MATCHES "Debug")
		set(GEG_SHADERC_LIB "$ENV{VULKAN_SDK}/Lib/shaderc_combinedd.lib")
	else()
		set(GEG_SHADERC_LIB "$ENV{VULKAN_SDK}/Lib/shaderc_combined.lib")
	endif()
	message(STATUS ${GEG_SHADERC_LIB})
else()
	message(FATAL_ERROR "install VulkanSDK, and add to the env variable VULKAN_SDK")
endif()
# ==============================

# ============== glfw =========================
if(WIN32)
	add_compile_definitions(_GLFW_WIN32=1)
elseif(UNIX AND NOT APPLE)
	add_compile_definitions(_GLFW_X11=1)
endif()


set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_VULKAN_STATIC ON CACHE BOOL "" FORCE)
add_subdirectory(vendor/glfw)
# ==============================================

# =============== assimp ============
set(ASSIMP_NO_EXPORT OFF CACHE BOOL "" FORCE) 
set(ASSIMP_BUILD_TESTS OFF CACHE BOOL "" FORCE) 
add_subdirectory(vendor/assimp)
# ===================================

add_subdirectory(vendor/spdlog)
add_subdirectory(vendor/glm)
add_subdirectory(vendor/imgui)
add_subdirectory(vendor/vma)
add_subdirectory(vendor/spirv/reflect)
add_subdirectory(vendor/entt)

# ================== geg ====================
# find all source files and headers
file(
	GLOB_RECURSE # recursive
	GEG_SRC # variable to store the source files and headers
	CONFIGURE_DEPENDS # make a dependency
	"${PROJECT_SOURCE_DIR}/src/*.hpp" "${PROJECT_SOURCE_DIR}/src/*.cpp"
)
add_library(geg STATIC ${GEG_SRC})
# ===========================================

# =============== deps ======================
# vulkan .. vars should be defined by find vulkan or manually
target_link_libraries(geg PUBLIC Vulkan::Vulkan) # loader
target_link_libraries(geg PUBLIC ${GEG_SHADERC_LIB})

target_link_libraries(geg PUBLIC spdlog)
target_link_libraries(geg PUBLIC glfw)
target_link_libraries(geg PUBLIC imgui)
target_link_libraries(geg PUBLIC glm)
target_link_libraries(geg PUBLIC VulkanMemoryAllocator)
target_link_libraries(geg PUBLIC sprivReflect)
target_link_libraries(geg PUBLIC assimp)
target_link_libraries(geg PUBLIC entt)
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
