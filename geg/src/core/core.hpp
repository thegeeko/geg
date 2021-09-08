#ifndef GEG_CORE_HPP
#define GEG_CORE_HPP

#include "logger.hpp"

#define GEG_ENABLE_ASSERT

#ifdef GEG_ENABLE_ASSERT
	#define GEG_CORE_ASSERT(toCheck, ...) {if (!(toCheck)) { GEG_CORE_ERROR("Assert failed {}", __VA_ARGS__);}}
	#define GEG_ASSERT(toCheck, ...) {if (!(toCheck)) { GEG_ERROR("Assert failed {}", __VA_ARGS__);}}
#else
	#define GEG_CORE_ASSERT(...)
	#define GEG_ASSERT(...)
#endif

#endif //GEG_CORE_HPP
