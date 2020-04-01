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
	//#error "Linux is not supported!"
#else
	/*Unknown compiler or platform*/
	#error "unknown platform!"
#endif // _WIN32


#if defined NC_PLATFORM_WINDOWS
	#if NC_DYNAMIC_LINK
		#ifdef NC_BUILD_DLL
			#define NUCLEUS_API __declspec(dllexport)
		#else
			#define NUCLEUS_API __declspec(dllimport)
		#endif //NC_BUILD_DLL
	#else
		#define NUCLEUS_API
	#endif

#elif defined NC_PLATFORM_LINUX
	#if NC_DYNAMIC_LINK
		#ifdef NC_BUILD_DLL
			#define NUCLEUS_API __attribute__((visibility("default")))
		#else
			#define NUCLEUS_API
		#endif
	#else
		#define NUCLEUS_API
	#endif
#else
	#error Nucleus only supports Windows at the moment!	
#endif //NC_PLATFORM_WINDOWS

#if defined NC_DEBUG
	#if defined NC_PLATFORM_WINDOWS
		#define NC_DEBUGBREAK() __debugbreak()
	#elif defined NC_PLATFORM_LINUX
		#include <signal.h>
		#define NC_DEBUGBREAK() raise(SIGTRAP)
	#endif 

	#define NC_ENABLE_ASSERTS
#endif //NC_DEBUG

#ifdef NC_ENABLE_ASSERTS
	#define NC_ASSERT(x, ...) {if(!(x)) { NC_ERROR("Assertion Failed: {0}", __VA_ARGS__); NC_DEBUGBREAK(); }}
	#define NC_CORE_ASSERT(x, ...) {if(!(x)) { NC_CORE_ERROR("Assertion Failed: {0}", __VA_ARGS__); NC_DEBUGBREAK(); }}
#else
	#define NC_ASSERT(x, ...)
	#define NC_CORE_ASSERT(x, ...)
#endif // NC_ENABLE_ASSERTS


#define BIT(x) (1 << x)

#define BIND_EVENT_FN(fn) std::bind(&fn, this, std::placeholders::_1)

namespace Nucleus {
	
	template<typename T>
	using Scope = std::unique_ptr<T>;
	template<typename T, typename ... Args>
	constexpr Scope<T> CreateScope(Args&& ... args) 
	{
		return std::make_unique<T>(std::forward<Args>(args) ...);
	}
	
	template<typename T>
	using Ref = std::shared_ptr<T>;
	template<typename T, typename ... Args>
	constexpr Ref<T> CreateRef(Args&& ... args)
	{
		return std::make_shared<T>(std::forward<Args>(args) ...);
	}
}