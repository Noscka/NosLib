#include "NosLib/Console.hpp"
#include "NosLib/HashTable.hpp"

#include <windows.h>
#include <conio.h>
#include <cstddef>
#include <cassert>

/* TODO: Figure out if it is worth it to change calling convention from default (__cdelc) to __fastcall */

class SimpleTestClass
{
protected:
	int Num1;
	int Num2;
	std::wstring Name;
public:
	SimpleTestClass(){}

	SimpleTestClass(const int& num1, const int& num2, const std::wstring& name)
	{
		Num1 = num1;
		Num2 = num2;
		Name = name;
	}

	std::wstring GetKey()
	{
		return Name;
	}
};

int main()
{
	NosLib::Console::InitializeModifiers::EnableUnicode();
	NosLib::Console::InitializeModifiers::EnableANSI();
	NosLib::Console::InitializeModifiers::BeatifyConsole<wchar_t>(L"Hash Table");
	NosLib::Console::InitializeModifiers::InitializeEventHandler();

	NosLib::HashTable<std::wstring, SimpleTestClass**> testHashTable(&SimpleTestClass::GetKey);

	SimpleTestClass* megaPointer1 = new SimpleTestClass(1, 2, L"NiceName");
	SimpleTestClass** megaPointer2 = &megaPointer1;

	testHashTable.Insert(megaPointer2);

	SimpleTestClass*** object = testHashTable.Find(L"NiceName");

	if (object == nullptr)
	{
		wprintf(L"Not found\n");
	}
	else
	{
		wprintf(L"found object\n");
	}

	wprintf((testHashTable.Remove(L"NiceName") ? L"Removed Successfully\n" : L"Not Removed\n"));

	wprintf(L"Press any button to continue"); _getch();
	return 0;
}