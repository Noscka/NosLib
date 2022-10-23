#include <NosStdLib/DynamicMenuSystem.hpp>
#include <NosStdLib/TestEnviroment.hpp>
#include <NosStdLib/Global.hpp>

#include <iostream>
#include <io.h>
#include <fcntl.h>

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
    _setmode(_fileno(stdout), _O_U16TEXT);

    NosStdLib::Menu::DynamicMenu MainMenu = NosStdLib::Menu::DynamicMenu(L"Main Menu", false, true, true);
    MainMenu.AddMenuEntry(NosStdLib::Menu::MenuEntry(L"========== Boolean =========="));

    MainMenu.AddMenuEntry(NosStdLib::Menu::MenuEntry(L"Toggle", &SomeBool));
    MainMenu.AddMenuEntry(NosStdLib::Menu::MenuEntry(L"Check Bool", CheckBool));

    MainMenu.AddMenuEntry(NosStdLib::Menu::MenuEntry(L"========== Integer =========="));

    MainMenu.AddMenuEntry(NosStdLib::Menu::MenuEntry(L"Number", &number));
    MainMenu.AddMenuEntry(NosStdLib::Menu::MenuEntry(L"Check Number", CheckNumber));

    NosStdLib::Menu::DynamicMenu SecondaryMenu = NosStdLib::Menu::DynamicMenu(L"Second Menu", false, true, true);

    SecondaryMenu.AddMenuEntry(NosStdLib::Menu::MenuEntry(L"Number", &number));
    SecondaryMenu.AddMenuEntry(NosStdLib::Menu::MenuEntry(L"Check Number", CheckNumber));

    MainMenu.AddMenuEntry(NosStdLib::Menu::MenuEntry(L"Another Menu", &SecondaryMenu));

    MainMenu.StartMenu();

    wprintf(L"Press any button to continue"); getchar();
    return 0;
}