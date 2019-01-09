#pragma once

#ifdef NC_PLATFORM_WINDOWS
	#ifdef NC_BUILD_DLL
		#define NUCLEUS_API __declspec(dllexport)
	#else
		#define NUCLEUS_API __declspec(dllimport)
	#endif //NC_BUILD_DLL
#else
	#error Nucleus only supports Windows at the moment!	
#endif //NC_PLATFORM_WINDOWS
