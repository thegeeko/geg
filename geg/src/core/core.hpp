#pragma once

#define GEG_ENABLE_ASSERT
#define GLFW_INCLUDE_NONE

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

namespace Geg::Utils {
	std::string readFileAsString(const std::string &filePath);
}		 // namespace Geg::Utils
