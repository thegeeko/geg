cmake_minimum_required(VERSION 3.16)
project(geg)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake/modules")

# add_compile_definitions("NDEBUG")

add_subdirectory(geg)
add_subdirectory(sandbox)


execute_process(
	COMMAND ${CMAKE_COMMAND} -E create_symlink
		${CMAKE_BINARY_DIR}/compile_commands.json
		${CMAKE_SOURCE_DIR}/compile_commands.json
)
