#pragma once

#if defined(_WIN32) && defined(NOSLIB_SHARED)
	#ifdef NOSLIB_EXPORT
		#define NOSLIB_API __declspec(dllexport)
	#else
		#define NOSLIB_API __declspec(dllimport)
	#endif
#else
	#define NOSLIB_API
#endif