#ifndef _STRING_INTERNAL_NOSLIB_HPP_
#define _STRING_INTERNAL_NOSLIB_HPP_

#include <string>

namespace NosLib
{
	#ifdef UNICODE
	using NosChar = wchar_t;
	#else
	using NosChar = char;
	#endif

	using NosString = std::basic_string<NosChar>;
	using NosString_View = std::basic_string_view<NosChar>;
}

#endif