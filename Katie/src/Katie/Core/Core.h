#pragma once

#ifdef KT_PLATFORM_WINDOWS
	#ifdef KT_BUILD_DLL
		#define KATIE_API __declspec(dllexport)
	#else
		#define KATIE_API __declspec(dllimport)
	#endif // KT_BUILD_DLL
#else
	#error Katie only support Windows!
#endif