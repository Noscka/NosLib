#pragma once

#ifdef _WIN32
	#ifdef NOSLIB_EXPORTS
		#define NOSLIB_API __declspec(dllexport)
	#else
		#define NOSLIB_API __declspec(dllimport)
	#endif
#else
	#define NOSLIB_API
#endif