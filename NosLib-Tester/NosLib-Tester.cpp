#include "NosLib/Console.hpp"
#include "NosLib/HashTable.hpp"

#include <windows.h>
#include <conio.h>
#include <cstddef>
#include <cassert>

/* TODO: Figure out if it is worth it to change calling convention from default (__cdelc) to __fastcall */

enum FlagTesting
{
	Flag1 = 1 << 0,
	Flag2 = 1 << 1,
	Flag3 = 1 << 2,
	Flag4 = 1 << 3,
};
DEFINE_ENUM_FLAG_OPERATORS(FlagTesting)

int main()
{
	NosLib::Console::InitializeModifiers::EnableUnicode();
	NosLib::Console::InitializeModifiers::EnableANSI();
	NosLib::Console::InitializeModifiers::BeatifyConsole<wchar_t>(L"Basic Flag Testing");
	NosLib::Console::InitializeModifiers::InitializeEventHandler();

	FlagTesting flag = Flag1 | Flag2 | Flag3;

	if (flag & Flag1)
	{
		wprintf(L"Contains Flag1\n");
	}

	if (flag & Flag2)
	{
		wprintf(L"Contains Flag2\n");
	}

	if (flag & Flag3)
	{
		wprintf(L"Contains Flag3\n");
	}

	if (flag & Flag4)
	{
		wprintf(L"Contains Flag4\n");
	}

	wprintf(L"Press any button to continue"); _getch();
	return 0;
}