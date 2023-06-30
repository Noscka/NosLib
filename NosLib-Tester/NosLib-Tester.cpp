#include "NosLib/Console.hpp"
#include "NosLib/DynamicMenuSystem.hpp"
#include "NosLib/EventHandling/EventHandling.hpp"

#include <Windows.h>
#include <conio.h>

/* TODO: Figure out if it is worth it to change calling convention from default (__cdelc) to __fastcall */

void testFunction()
{
	wprintf(L"\nsup\n");
}

void waitFunction()
{
	wprintf(L"waiting...\n");
	_getch();
}

int main()
{
    NosLib::Console::InitializeModifiers::EnableUnicode();
    NosLib::Console::InitializeModifiers::EnableANSI();
    NosLib::Console::InitializeModifiers::BeatifyConsole<wchar_t>(L"Dynamic Array Updating");
    NosLib::Console::InitializeModifiers::InitializeEventHandler();

	NosLib::EventHandling::AddCleanupFunction(&testFunction);
	NosLib::EventHandling::AddCleanupFunction(&waitFunction);

	/*int variable1 = 0;

	NosLib::Menu::DynamicMenu mainMenu(L"Basic Menu");

	NosLib::Menu::DynamicMenu secondMenu(L"Basic Menu");

	mainMenu.AddMenuEntry(new NosLib::Menu::MenuEntry<NosLib::Menu::DynamicMenu>(L"second menu", &secondMenu));

	for (int i = 0; i <= 40; i++)
	{
		mainMenu.AddMenuEntry(new NosLib::Menu::MenuEntry<int>(std::format(L"int {}", i), &variable1));
	}

	mainMenu.StartMenu();*/

    wprintf(L"Press any button to continue"); _getch();
    return 0;
}