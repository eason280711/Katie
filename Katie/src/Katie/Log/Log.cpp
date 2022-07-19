#include "Log.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include "ktpch.h"

namespace Katie {
	std::shared_ptr<spdlog::logger> Log::s_CoreLogger;
	std::shared_ptr<spdlog::logger> Log::s_ClientLogger;

	void Log::Init()
	{
		spdlog::set_pattern("%^[%T] %n: %v%$");
		s_CoreLogger = spdlog::stdout_color_mt("KATIE");
		s_CoreLogger->set_level(spdlog::level::trace);

		s_ClientLogger = spdlog::stdout_color_mt("APP");
		s_ClientLogger->set_level(spdlog::level::trace);
	}
	Log::~Log()
	{
		s_CoreLogger->flush();
		s_ClientLogger->flush();
		spdlog::drop_all();
	}
}