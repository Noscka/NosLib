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
	NosLib::Console::InitializeModifiers::BeatifyConsole<wchar_t>(L"Array Addition Testing");
	NosLib::Console::InitializeModifiers::InitializeEventHandler();

	NosLib::DynamicArray<int> array1({1,2,3,4});
	NosLib::DynamicArray<int> array2(5);
	int basicArray[] = {10,20,30,40};
	array2.MultiAppend(basicArray, basicArray+4);

	for (int entry : array2)
	{
		std::wcout << entry << std::endl;
	}

	NosLib::DynamicArray<int> totalArray = array1 + array2;

	wprintf(L"=====================================================\n");

	for (int entry : totalArray)
	{
		std::wcout << entry << std::endl;
	}

	wprintf(L"Press any button to continue"); _getch();
	return 0;
}