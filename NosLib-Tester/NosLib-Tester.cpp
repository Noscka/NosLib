#include "NosLib/Console.hpp"
#include "NosLib/Functional.hpp"
#include "NosLib/DynamicMenuSystem.hpp"

#include <Windows.h>
#include <conio.h>

/* TODO: Figure out if it is worth it to change calling convention from default (__cdelc) to __fastcall */

bool SomeBool = false;
int number = 1;
void CheckBool()
{
	wprintf((SomeBool ? L"true\n" : L"false\n"));
	wprintf(L"Press any button to continue"); _getch();
	return;
}
void CheckNumber()
{
	wprintf((std::to_wstring(number) + L"\n").c_str());
	wprintf(L"Press any button to continue"); _getch();
	return;
}
void SomeFunction(int* param1, int* param2)
{
	std::wcout << L"Param1: " << *param1 << L" | Param2: " << *param2 << std::endl;
	wprintf(L"Press any button to continue"); _getch();
	return;
}
void PrintingFunction(std::wstring input)
{
	wprintf(input.c_str());
}

int main()
{
    NosLib::Console::InitializeModifiers::EnableUnicode();
    NosLib::Console::InitializeModifiers::EnableANSI();
    NosLib::Console::InitializeModifiers::BeatifyConsole<wchar_t>(L"Dynamic Array Updating");
    NosLib::Console::InitializeModifiers::InitializeEventHandler();

	NosLib::Menu::DynamicMenu MainMenu(L"Main Menu", true, true, true);
	NosLib::Menu::DynamicMenu SecondaryMenu(L"Second Menu", true, true, true);
	NosLib::Menu::DynamicMenu IteratedMenu(L"Iterated Menu", true, true, true);

	int sharedInt = 30;

	for (int i = 0; i <= 80; i++)
	{
		IteratedMenu.AddMenuEntry(new NosLib::Menu::MenuEntry(std::format(L"Iterated Int {}", i), &sharedInt));
	}

	SecondaryMenu.AddMenuEntry(new NosLib::Menu::MenuEntry(L"Number", &number));
	SecondaryMenu.AddMenuEntry(new NosLib::Menu::MenuEntry(L"Check Number", new NosLib::Functional::FunctionStore(&CheckNumber)));

	int param1 = 0,
		param2 = 10;

	SecondaryMenu.AddMenuEntry(new NosLib::Menu::MenuEntry(L"param1", &param1));
	SecondaryMenu.AddMenuEntry(new NosLib::Menu::MenuEntry(L"param2", &param2));

	SecondaryMenu.AddMenuEntry(new NosLib::Menu::MenuEntry(L"Run Function", new NosLib::Functional::FunctionStore(&SomeFunction, &param1, &param2)));

	MainMenu.AddMenuEntry(new NosLib::Menu::MenuEntry(L"Secondary Menu", &SecondaryMenu));
	MainMenu.AddMenuEntry(new NosLib::Menu::MenuEntry(L"Iterated Menu", &IteratedMenu));

	MainMenu.AddMenuEntry(new NosLib::Menu::MenuEntry(L"Toggle", &SomeBool));
	MainMenu.AddMenuEntry(new NosLib::Menu::MenuEntry(L"Check Bool", new NosLib::Functional::FunctionStore(&CheckBool)));

	MainMenu.AddMenuEntry(new NosLib::Menu::MenuEntry(L"Number", &number));
	MainMenu.AddMenuEntry(new NosLib::Menu::MenuEntry(L"Check Number", new NosLib::Functional::FunctionStore(&CheckNumber)));

	MainMenu.StartMenu();

    wprintf(L"Press any button to continue"); _getch();
    return 0;
}