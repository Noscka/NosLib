#include <NosStdLib/DynamicMenuSystem.hpp>
#include <NosStdLib/TestEnviroment.hpp>
#include <NosStdLib/Global.hpp>

#include <iostream>
#include <io.h>
#include <fcntl.h>
#include <cstdio>


bool SomeBool = false;
int number = 0;

void CheckBool()
{
    wprintf((SomeBool ? L"true" : L"false"));
    system("Pause");
}

void CheckNumber()
{
    wprintf(std::to_wstring(number).c_str());
    system("Pause");
}

int main()
{
    NosStdLib::Global::Console::InitializeModifiers::EnableUnicode();
    NosStdLib::Global::Console::InitializeModifiers::EnableANSI();

    NosStdLib::Menu::DynamicMenu MainMenu(L"Main Menu", false, true, true);
    NosStdLib::Menu::DynamicMenu SecondaryMenu(L"Second Menu", false, true, true);

    SecondaryMenu.AddMenuEntry(new NosStdLib::Menu::MenuEntry(L"Number", &number));
    SecondaryMenu.AddMenuEntry(new NosStdLib::Menu::MenuEntry(L"Check Number", CheckNumber));

    MainMenu.AddMenuEntry(new NosStdLib::Menu::MenuEntry(L"Another Menu", &SecondaryMenu));

    MainMenu.AddMenuEntry(new NosStdLib::Menu::MenuEntry(L"========== Boolean =========="));

    MainMenu.AddMenuEntry(new NosStdLib::Menu::MenuEntry(L"Toggle", &SomeBool));
    MainMenu.AddMenuEntry(new NosStdLib::Menu::MenuEntry(L"Check Bool", CheckBool));

    MainMenu.AddMenuEntry(new NosStdLib::Menu::MenuEntry(L"========== Integer =========="));

    MainMenu.AddMenuEntry(new NosStdLib::Menu::MenuEntry(L"Number", &number));
    MainMenu.AddMenuEntry(new NosStdLib::Menu::MenuEntry(L"Check Number", CheckNumber));

    MainMenu.StartMenu();

    wprintf(L"Press any button to continue"); getchar();
    return 0;
}