#include "NosLib/Console.hpp"
#include "NosLib/DynamicMenuSystem.hpp"

#include <Windows.h>
#include <conio.h>

/* TODO: Figure out if it is worth it to change calling convention from default (__cdelc) to __fastcall */

int main()
{
    NosLib::Console::InitializeModifiers::EnableUnicode();
    NosLib::Console::InitializeModifiers::EnableANSI();
    NosLib::Console::InitializeModifiers::BeatifyConsole<wchar_t>(L"Enum Testing");
    NosLib::Console::InitializeModifiers::InitializeEventHandler();

    int variable1 = 0;
    int variable2 = 0;
    int variable3 = 0;

    NosLib::Menu::DynamicMenu mainMenu(L"Basic Menu");

    NosLib::Menu::DynamicMenu secondMenu(L"Basic Menu");

    mainMenu.AddMenuEntry(new NosLib::Menu::MenuEntry<NosLib::Menu::DynamicMenu>(L"second menu", &secondMenu));

    mainMenu.AddMenuEntry(new NosLib::Menu::MenuEntry<int>(L"int 1", &variable1));
    mainMenu.AddMenuEntry(new NosLib::Menu::MenuEntry<int>(L"int 2", &variable2));
    mainMenu.AddMenuEntry(new NosLib::Menu::MenuEntry<int>(L"int 3", &variable3));

    mainMenu.StartMenu();

    wprintf(L"Press any button to continue"); _getch();
    return 0;
}