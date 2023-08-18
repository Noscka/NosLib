#include "NosLib/Console.hpp"
#include "NosLib/String.hpp"
#include "NosLib/Logging.hpp"
#include "NosLib/ErrorHandling.hpp"

#include <windows.h>
#include <conio.h>
#include <cstddef>
#include <cassert>

/* TODO: Figure out if it is worth it to change calling convention from default (__cdelc) to __fastcall */

int main()
{
	NosLib::Console::InitializeModifiers::EnableUnicode();
	NosLib::Console::InitializeModifiers::EnableANSI();
	NosLib::Console::InitializeModifiers::BeatifyConsole<wchar_t>(L"Nothing");
	NosLib::Console::InitializeModifiers::InitializeEventHandler();

	wprintf(L"Press any button to continue"); _getch();
	return 0;
}