#pragma once

#include "spdlog/spdlog.h"
#include <spdlog/logger.h>

#include <memory>

namespace Holloware 
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
#define HW_CORE_ERROR(...) ::Holloware::Log::GetCoreLogger()->error(__VA_ARGS__)
#define HW_CORE_WARN(...) ::Holloware::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define HW_CORE_INFO(...) ::Holloware::Log::GetCoreLogger()->info(__VA_ARGS__)
#define HW_CORE_TRACE(...) ::Holloware::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define HW_CORE_FATAL(...) ::Holloware::Log::GetCoreLogger()->fatal(__VA_ARGS__)

// Client log macros
#define HW_ERROR(...) ::Holloware::Log::GetClientLogger()->error(__VA_ARGS__)
#define HW_WARN(...) ::Holloware::Log::GetClientLogger()->warn(__VA_ARGS__)
#define HW_INFO(...) ::Holloware::Log::GetClientLogger()->info(__VA_ARGS__)
#define HW_TRACE(...) ::Holloware::Log::GetClientLogger()->trace(__VA_ARGS__)
#define HW_FATAL(...) ::Holloware::Log::GetClientLogger()->fatal(__VA_ARGS__)