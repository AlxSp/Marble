#pragma once

#include "ncpch.h"
#include "Core.h"
#include "spdlog/spdlog.h"
#include "spdlog/fmt/ostr.h"

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
#define NC_CORE_CRITICAL(...)   ::Nucleus::Log::GetCoreLogger()->critical(__VA_ARGS__)

//Client log macros
#define NC_TRACE(...)        ::Nucleus::Log::GetClientLogger()->trace(__VA_ARGS__)
#define NC_INFO(...)         ::Nucleus::Log::GetClientLogger()->info(__VA_ARGS__)
#define NC_WARNING(...)      ::Nucleus::Log::GetClientLogger()->warn(__VA_ARGS__)
#define NC_ERROR(...)        ::Nucleus::Log::GetClientLogger()->error(__VA_ARGS__)
#define NC_FATAL_CRITICAL(...)        ::Nucleus::Log::GetClientLogger()->critical(__VA_ARGS__)
