#pragma once

#include <memory.h>
#include "Core.h"
#include "spdlog/spdlog.h"

namespace Nucleus {
	class NUCLEUS_API Log
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

//Core log macros
#define NC_CORE_TRACE(...)   ::Nucleus::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define NC_CORE_INFO(...)    ::Nucleus::Log::GetCoreLogger()->info(__VA_ARGS__)
#define NC_CORE_WARNING(...) ::Nucleus::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define NC_CORE_ERROR(...)   ::Nucleus::Log::GetCoreLogger()->error(__VA_ARGS__)
#define NC_CORE_FATAL(...)   ::Nucleus::Log::GetCoreLogger()->fatal(__VA_ARGS__)

//Client log macros
#define NC_TRACE(...)        ::Nucleus::Log::GetClientLogger()->trace(__VA_ARGS__)
#define NC_INFO(...)         ::Nucleus::Log::GetClientLogger()->info(__VA_ARGS__)
#define NC_WARNING(...)      ::Nucleus::Log::GetClientLogger()->warning(__VA_ARGS__)
#define NC_ERROR(...)        ::Nucleus::Log::GetClientLogger()->error(__VA_ARGS__)
#define NC_FATAL(...)        ::Nucleus::Log::GetClientLogger()->fatal(__VA_ARGS__)
