#include "logger.hpp"

namespace geg {

	std::shared_ptr<spdlog::logger> Logger::coreLogger;
	std::shared_ptr<spdlog::logger> Logger::appLogger;

	void Logger::init() {
		spdlog::set_pattern("%^[%T] %n: %v%$");
		coreLogger = spdlog::stderr_color_mt("geg");
		coreLogger->set_level(spdlog::level::trace);

		appLogger = spdlog::stderr_color_mt("app");
		appLogger->set_level(spdlog::level::trace);
	}

}