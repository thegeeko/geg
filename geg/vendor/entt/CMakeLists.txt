add_library(entt INTERFACE)
target_sources(entt INTERFACE entt.hpp)

target_include_directories(entt INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})