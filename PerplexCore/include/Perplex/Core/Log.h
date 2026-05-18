#pragma once

#define SPDLOG_USE_STD_FORMAT
#include <spdlog/spdlog.h>
#include <spdlog/logger.h>

#include <memory>

namespace Perplex 
{
	class Log
	{
	public:
		static void Init();

		inline static std::shared_ptr<spdlog::logger>& GetCoreLogger() { return s_CoreLogger; }
		inline static std::shared_ptr<spdlog::logger>& GetClientLogger() { return s_ClientLogger; }

	private:
		static std::shared_ptr<spdlog::logger> s_CoreLogger;
		static std::shared_ptr<spdlog::logger> s_ClientLogger;
	};
}

// Core log macros
#define HW_CORE_ERROR(...) ::Perplex::Log::GetCoreLogger()->error(__VA_ARGS__)
#define HW_CORE_WARN(...) ::Perplex::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define HW_CORE_INFO(...) ::Perplex::Log::GetCoreLogger()->info(__VA_ARGS__)
#define HW_CORE_TRACE(...) ::Perplex::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define HW_CORE_FATAL(...) ::Perplex::Log::GetCoreLogger()->fatal(__VA_ARGS__)

// Client log macros
#define HW_ERROR(...) ::Perplex::Log::GetClientLogger()->error(__VA_ARGS__)
#define HW_WARN(...) ::Perplex::Log::GetClientLogger()->warn(__VA_ARGS__)
#define HW_INFO(...) ::Perplex::Log::GetClientLogger()->info(__VA_ARGS__)
#define HW_TRACE(...) ::Perplex::Log::GetClientLogger()->trace(__VA_ARGS__)
#define HW_FATAL(...) ::Perplex::Log::GetClientLogger()->fatal(__VA_ARGS__)