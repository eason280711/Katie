#pragma once
#include "Katie/Core/Core.h"
#include "spdlog/spdlog.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include "spdlog/fmt/ostr.h"
#include "ktpch.h"


namespace Katie {
	class KATIE_API Log
	{
	public:
		static void Init();
		~Log();

		inline static std::shared_ptr<spdlog::logger>& GetCoreLogger() { return s_CoreLogger; }
		inline static std::shared_ptr<spdlog::logger>& GetClientLogger() { return s_ClientLogger; }
	private:
		static std::shared_ptr<spdlog::logger> s_CoreLogger;
		static std::shared_ptr<spdlog::logger> s_ClientLogger;
	};
}

// Core Log macro
#define KT_CORE_TRACE(...) ::Katie::Log::GetCoreLogger()->trace(__VA_ARGS__);
#define KT_CORE_INFO(...) ::Katie::Log::GetCoreLogger()->info(__VA_ARGS__);
#define KT_CORE_WARN(...) ::Katie::Log::GetCoreLogger()->warn(__VA_ARGS__);
#define KT_CORE_ERROR(...) ::Katie::Log::GetCoreLogger()->error(__VA_ARGS__);
#define KT_CORE_FATAL(...) ::Katie::Log::GetCoreLogger()->fatal(__VA_ARGS__);

// Client Log macro
#define KT_TRACE(...) ::Katie::Log::GetClientLogger()->trace(__VA_ARGS__);
#define KT_INFO(...) ::Katie::Log::GetClientLogger()->info(__VA_ARGS__);
#define KT_WARN(...) ::Katie::Log::GetClientLogger()->warn(__VA_ARGS__);
#define KT_ERROR(...) ::Katie::Log::GetClientLogger()->error(__VA_ARGS__);
#define KT_FATAL(...) ::Katie::Log::GetClientLogger()->fatal(__VA_ARGS__);