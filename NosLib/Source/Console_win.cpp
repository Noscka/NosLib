#ifdef _WIN32
#include <NosLib/Console.hpp>

namespace NosLib
{
	void ClearScreen()
	{
		HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
		if (hStdOut == INVALID_HANDLE_VALUE) return;

		CONSOLE_SCREEN_BUFFER_INFO csbi;
		if (!GetConsoleScreenBufferInfo(hStdOut, &csbi)) return;

		DWORD cellCount = csbi.dwSize.X * csbi.dwSize.Y;
		DWORD count;
		COORD homeCoords = { 0, 0 };

		FillConsoleOutputCharacter(hStdOut, ' ', cellCount, homeCoords, &count);
		FillConsoleOutputAttribute(hStdOut, csbi.wAttributes, cellCount, homeCoords, &count);
		SetConsoleCursorPosition(hStdOut, homeCoords);
	}
}

#endif