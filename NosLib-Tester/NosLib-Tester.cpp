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
	NosLib::Console::InitializeModifiers::BeatifyConsole<wchar_t>(L"Insert Testing");
	NosLib::Console::InitializeModifiers::InitializeEventHandler();

	NosLib::DynamicArray<std::wstring> insertTesting(11);

	for (int i = 0; i <= 10; i++)
	{
		insertTesting.Append(std::format(L"number: {}\n", i));
	}

	for (std::wstring entry : insertTesting)
	{
		wprintf(entry.c_str());
	}

	wprintf(std::format(L"count: {}\n\n", insertTesting.GetItemCount()).c_str());

	insertTesting.Insert(L"Added Item\n", 0);

	for (std::wstring entry : insertTesting)
	{
		wprintf(entry.c_str());
	}
	wprintf(std::format(L"count: {}\n", insertTesting.GetItemCount()).c_str());

	wprintf(L"Press any button to continue"); _getch();
	return 0;
}