#ifndef _WIN32
#include <NosLib/Console.hpp>

namespace NosLib
{
	void ClearScreen()
	{
		printf("\033[2J\033[H");
	}
}

#endif