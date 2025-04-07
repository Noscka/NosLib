#ifdef _WIN32
#include <NosLib/Console.hpp>
#include <Windows.h>

namespace NosLib
{
	bool EnableANSI()
	{
		HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
		if (consoleHandle == INVALID_HANDLE_VALUE)
		{
			return false;
		}

		DWORD consoleMode = 0;
		bool getOperationResults = GetConsoleMode(consoleHandle, &consoleMode);
		consoleMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING; /* makes output get parsed, so ANSI gets actually used */
		consoleMode |= DISABLE_NEWLINE_AUTO_RETURN; /* makes it so newlines have an effect. */
		bool setOperationResults = SetConsoleMode(consoleHandle, consoleMode);

		return (getOperationResults && setOperationResults);
	}

	bool CheckANSI()
	{
		HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
		DWORD consoleMode = 0;

		if (consoleHandle == INVALID_HANDLE_VALUE)
		{
			return false;
		}

		if (!GetConsoleMode(consoleHandle, &consoleMode))
		{
			return false;
		}
		
		return (consoleMode & ENABLE_VIRTUAL_TERMINAL_PROCESSING) != 0;
	}

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