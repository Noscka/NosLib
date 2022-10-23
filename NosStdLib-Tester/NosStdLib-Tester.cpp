#include <NosStdLib/DynamicMenuSystem.hpp>
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

    NosStdLib::Menu::DynamicMenu MainMenu(L"Main Menu", false, true, true);
    NosStdLib::Menu::DynamicMenu SubMenu(L"Sub Menu", false, true, true);

    SubMenu.AddMenuEntry(NosStdLib::Menu::MenuEntry(L"Number", &number));
    SubMenu.AddMenuEntry(NosStdLib::Menu::MenuEntry(L"Check Number", CheckNumber));

    MainMenu.AddMenuEntry(NosStdLib::Menu::MenuEntry(L"Secondary Menu", &SubMenu));

    MainMenu.AddMenuEntry(NosStdLib::Menu::MenuEntry(L"========== Boolean =========="));

    MainMenu.AddMenuEntry(NosStdLib::Menu::MenuEntry(L"Toggle", &SomeBool));
    MainMenu.AddMenuEntry(NosStdLib::Menu::MenuEntry(L"Check Bool", CheckBool));

    MainMenu.AddMenuEntry(NosStdLib::Menu::MenuEntry(L"========== Integer =========="));

    MainMenu.AddMenuEntry(NosStdLib::Menu::MenuEntry(L"Number", &number));
    MainMenu.AddMenuEntry(NosStdLib::Menu::MenuEntry(L"Check Number", CheckNumber));

    MainMenu.StartMenu();

    wprintf(L"Press any button to continue"); getchar();
    return 0;
}