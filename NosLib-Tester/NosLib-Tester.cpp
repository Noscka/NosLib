#include "NosLib/Console.hpp"
#include "NosLib/String.hpp"

#include "AllHeaders.hpp"

#include <Windows.h>
#include <conio.h>

/* TODO: Figure out if it is worth it to change calling convention from default (__cdelc) to __fastcall */

int main()
{
	NosLib::Console::InitializeModifiers::EnableUnicode();
	NosLib::Console::InitializeModifiers::EnableANSI();
	NosLib::Console::InitializeModifiers::BeatifyConsole<wchar_t>(L"Shorten String Testing");
	NosLib::Console::InitializeModifiers::InitializeEventHandler();

	wprintf(L"Press any button to continue\n"); _getch();
	std::wstring stringToShorten(158, L'█');
	
	wprintf(NosLib::String::Shorten(stringToShorten).c_str());

	wprintf(L"Press any button to continue"); _getch();
	return 0;
}