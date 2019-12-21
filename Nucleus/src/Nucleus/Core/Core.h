#pragma once

#include <memory>

#ifdef _WIN32
	/*Windows x64/x86*/
	#ifdef _WIN64
		/*Windows x64*/
		#define NC_PLATFORM_WINDOWS
	#else
		/*Windows x86*/
		#error "x86 Builds are not supported"
	#endif // _WIN64
#elif defined(__APPLE__) || defined(__MACH__)
	#include <TargetConditionals.h>
	/* TARGET_OS_MAC exists on all the platforms
	 * so we must check all of them (in this order)
	 * to ensure that we're running on MAC
	 * and not some other Apple platform */
	#if TARGET_IPHONE_SIMULATOR == 1
		#error "IOS simulator is not supported!"
	#elif TARGET_OS_IPHONE == 1
		#define NC_PLATFORM_IOS
	#error "IOS is not supported!"
		#elif TARGET_OS_MAC == 1
	#define NC_PLATFORM_MACOS
		#error "MacOS is not supported!"
	#else
		#error "Unknown Apple platform!"
	#endif
#elif defined(__ANDROID__)
	#define NC_PLATFORM_ANDROID
	#error "Android is not supported!"
#elif defined(__linux__)
	#define NC_PLATFORM_LINUX
	#error "Linux is not supported!"
#else
	/*Unknown compiler or platform*/
	#error "unknown platform!"
#endif // _WIN32


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