#ifndef _BOOSTDEFINITIONS_NOSLIB_HPP_
#define _BOOSTDEFINITIONS_NOSLIB_HPP_
#ifdef NOSLIB_USE_BOOST

#include <NosLib/Boost/WinVersion.hpp>

#include <string>

namespace NosLib::Net
{
	namespace Definitions
	{
		constexpr uint64_t SegmentSize = 256000; /* Segments for sending/reading large objects and files */
		typedef unsigned char Byte;
	}
}

#endif // NOSLIB_USE_BOOST
#endif // _BOOSTDEFINITIONS_NOSLIB_HPP_