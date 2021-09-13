#ifndef GEG_LOGGER_HPP
#define GEG_LOGGER_HPP

#include <memory>
#include "spdlog/spdlog.h"
#include "spdlog/sinks/stdout_color_sinks.h"

namespace Geg {
	class Logger {
	public:
		static void init();

		inline static std::shared_ptr<spdlog::logger>& getCoreLogger () {return coreLogger;}
		inline static std::shared_ptr<spdlog::logger>& getAppLogger () {return appLogger;}

	private:
		static std::shared_ptr<spdlog::logger> coreLogger;
		static std::shared_ptr<spdlog::logger> appLogger;
	};
}

#define GEG_CORE_TRACE(...) Geg::Logger::getCoreLogger()->trace (__VA_ARGS__)
#define GEG_CORE_INFO(...) 	Geg::Logger::getCoreLogger()->info	(__VA_ARGS__)
#define GEG_CORE_WARN(...) 	Geg::Logger::getCoreLogger()->warn	(__VA_ARGS__)
#define GEG_CORE_ERROR(...) Geg::Logger::getCoreLogger()->error (__VA_ARGS__)
#define GEG_CORE_FETAL(...) Geg::Logger::getCoreLogger()->fetal (__VA_ARGS__)

#define GEG_TRACE(...) 	Geg::Logger::getAppLogger()->trace	(__VA_ARGS__)
#define GEG_INFO(...)		Geg::Logger::getAppLogger()->info	(__VA_ARGS__)
#define GEG_WARN(...) 	Geg::Logger::getAppLogger()->warn	(__VA_ARGS__)
#define GEG_ERROR(...) 	Geg::Logger::getAppLogger()->error	(__VA_ARGS__)
#define GEG_FETAL(...) 	Geg::Logger::getAppLogger()->fetal	(__VA_ARGS__)

#endif //GEG_LOGGER_HPP
