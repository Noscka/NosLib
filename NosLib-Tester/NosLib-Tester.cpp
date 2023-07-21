#include "NosLib/Console.hpp"
#include "NosLib/FileManagement.hpp"

#include "AllHeaders.hpp"

#include <Windows.h>
#include <conio.h>

/* TODO: Figure out if it is worth it to change calling convention from default (__cdelc) to __fastcall */

int main()
{
	NosLib::Console::InitializeModifiers::EnableUnicode();
	NosLib::Console::InitializeModifiers::EnableANSI();
	NosLib::Console::InitializeModifiers::BeatifyConsole<wchar_t>(L"Trim\\Reduce Testing");
	NosLib::Console::InitializeModifiers::InitializeEventHandler();

	std::wstring megaSpace = L"          afagf an        gagawg   gwagwa  aw sss  sssssssss     \t\t\t\t\t       ";

	wprintf(megaSpace.c_str());
	wprintf(L"||\n");
	wprintf(NosLib::String::Reduce<wchar_t>(megaSpace).c_str());
	wprintf(L"||\n");

	wprintf(L"Press any button to continue"); _getch();
	return 0;
}