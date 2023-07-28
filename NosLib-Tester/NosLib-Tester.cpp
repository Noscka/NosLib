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
	NosLib::Console::InitializeModifiers::BeatifyConsole<wchar_t>(L"File Shortcut Testing");
	NosLib::Console::InitializeModifiers::InitializeEventHandler();

	NosLib::DynamicArray<std::wstring> basicArray({L"a", L"b", L"c"});
	NosLib::DynamicArray<std::wstring> copyObjectArray = basicArray.ObjectExclude(L"b");
	NosLib::DynamicArray<std::wstring> copyIndexArray = basicArray.Exclude(1);

	std::wcout << basicArray << std::endl;
	std::wcout << copyObjectArray << std::endl;
	std::wcout << copyIndexArray << std::endl;

	wprintf(L"Press any button to continue"); _getch();
	return 0;
}