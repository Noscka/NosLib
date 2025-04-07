#ifndef _WIN32
#include <NosLib/Console.hpp>

#include <iostream>

namespace NosLib
{
	bool EnableANSI()
	{
		return true; /* Linux has it on by default */
	}

	bool CheckANSI()
	{
		return true; /* Linux always has ANSI enabled */
	}

	void ClearScreen()
	{
		printf("\033[2J\033[H");
	}
}

#endif