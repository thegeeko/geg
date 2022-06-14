#pragma once

#include <cstdint>
#include <memory>
#include <vector>

#define GEG_ENABLE_ASSERT

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#define GLFW_INCLUDE_NONE

#define GEG_USE_VULKAN

#ifdef GEG_API_OPENGL
	#ifndef GEG_API_VULKAN
	#endif
#endif

#ifndef NDEBUG
	#define GEG_DEBUG_ENABLED
#endif

#ifdef GEG_ENABLE_ASSERT
	#define GEG_CORE_ASSERT(toCheck, ...)                  \
		{                                                    \
			if (!(toCheck)) {                                  \
				GEG_CORE_ERROR("Assert failed {}", __VA_ARGS__); \
			}                                                  \
		}
	#define GEG_ASSERT(toCheck, ...)                  \
		{                                               \
			if (!(toCheck)) {                             \
				GEG_ERROR("Assert failed {}", __VA_ARGS__); \
			}                                             \
		}
#else
	#define GEG_CORE_ASSERT(...)
	#define GEG_ASSERT(...)
#endif

#define GEG_BIND_CB(fn)                                     \
	[this](auto &&...args) -> decltype(auto) {                \
		return this->fn(std::forward<decltype(args)>(args)...); \
	}

namespace geg {

	template<typename T>
	using Scope = std::unique_ptr<T>;

	template<typename T>
	using Ref = std::shared_ptr<T>;

	enum class GraphicsAPI {
		None = 0,
		OpenGL = 1,
		Vulkan = 2,
	};

	namespace Utils {
		template<typename T, typename... Rest>
		void hashCombine(std::size_t &seed, const T &v, const Rest &...rest);

		std::string readFileAsString(const std::string &filePath);
		std::vector<char> readFileAsBinary(const std::string &filePath);
	}		 // namespace Utils

}		 // namespace geg

#ifdef GEG_USE_VULKAN
const geg::GraphicsAPI GEG_CURRENT_API = geg::GraphicsAPI::Vulkan;
#endif
