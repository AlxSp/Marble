#pragma once

#include <memory>

#ifdef NC_PLATFORM_WINDOWS
#if NC_DYNAMIC_LINK
	#ifdef NC_BUILD_DLL
		#define NUCLEUS_API __declspec(dllexport)
	#else
		#define NUCLEUS_API __declspec(dllimport)
	#endif //NC_BUILD_DLL
#else
	#define NUCLEUS_API
#endif
#else
	#error Nucleus only supports Windows at the moment!	
#endif //NC_PLATFORM_WINDOWS

#ifdef NC_DEBUG
	#define NC_ENABLE_ASSERTS
#endif //NC_DEBUG

#ifdef NC_ENABLE_ASSERTS
	#define NC_ASSERT(x, ...) {if(!(x)) { NC_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); }}
	#define NC_CORE_ASSERT(x, ...) {if(!(x)) { NC_CORE_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); }}
#else
	#define NC_ASSERT(x, ...)
	#define NC_CORE_ASSERT(x, ...)
#endif // NC_ENABLE_ASSERTS


#define BIT(x) (1 << x)

#define BIND_EVENT_FN(fn) std::bind(&fn, this, std::placeholders::_1)

namespace Nucleus {
	
	template<typename T>
	using Scope = std::unique_ptr<T>;
	
	template<typename T>
	using Ref = std::shared_ptr<T>;
}