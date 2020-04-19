#pragma once

#include "mblpch.h"
#include "Marble/Core/Core.h"
#include <spdlog/spdlog.h>
#include <spdlog/fmt/ostr.h>

namespace Marble {
	class Log
	{
	public:
		static void Init();

		inline static Ref<spdlog::logger>& GetCoreLogger() { return s_CoreLogger; }
		inline static Ref<spdlog::logger>& GetClientLogger() { return s_ClientLogger; }

	private:
		static Ref<spdlog::logger> s_CoreLogger;
		static Ref<spdlog::logger> s_ClientLogger;
	};
}

//Core log macros
#define MBL_CORE_TRACE(...)		::Marble::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define MBL_CORE_INFO(...)		::Marble::Log::GetCoreLogger()->info(__VA_ARGS__)
#define MBL_CORE_WARN(...)		::Marble::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define MBL_CORE_ERROR(...)		::Marble::Log::GetCoreLogger()->error(__VA_ARGS__)
#define MBL_CORE_CRITICAL(...)	::Marble::Log::GetCoreLogger()->critical(__VA_ARGS__)

//Client log macros
#define MBL_TRACE(...)			::Marble::Log::GetClientLogger()->trace(__VA_ARGS__)
#define MBL_INFO(...)			::Marble::Log::GetClientLogger()->info(__VA_ARGS__)
#define MBL_WARN(...)			::Marble::Log::GetClientLogger()->warn(__VA_ARGS__)
#define MBL_ERROR(...)			::Marble::Log::GetClientLogger()->error(__VA_ARGS__)
#define MBL_FATAL_CRITICAL(...)	::Marble::Log::GetClientLogger()->critical(__VA_ARGS__)
