#include <NosStdLib/Global.hpp>
#include <NosStdLib/TestEnviroment.hpp>

#include <iostream>
#include <io.h>
#include <fcntl.h>
#include <cstdio>



int main()
{
    NosStdLib::Global::Console::InitializeModifiers::EnableUnicode();
    NosStdLib::Global::Console::InitializeModifiers::EnableANSI();

    //std::wcout << NosStdLib::TextColor::MakeANSICode<wchar_t>(NosStdLib::TextColor::NosRGB(140, 20, 30)) << L"abc" << L"\033[0m" << std::endl;

    NosStdLib::TestEnv::IterateSquare();

    wprintf(L"Press any button to continue"); getchar();
    return 0;
}

/* ANSI TEST
std::wcout << L"🯰" << std::endl;
std::wcout << L"\033[48;2;255;255;255;38;2;20;120;20m" << L"text 1" << L"\033[0m" << std::endl;
std::wcout << L"\033[38;5;214;48;5;124m" << L"text 2" << L"\033[0m"<< std::endl;
std::wcout << L"\033[5m" << L"text 2" << L"\033[0m"<< std::endl;
*/

/* MENU TEST
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

* NosStdLib::Menu::DynamicMenu MainMenu(L"Main Menu", false, true, true);
* NosStdLib::Menu::DynamicMenu SecondaryMenu(L"Second Menu", false, true, true);
* 
* SecondaryMenu.AddMenuEntry(new NosStdLib::Menu::MenuEntry(L"Number", &number));
* SecondaryMenu.AddMenuEntry(new NosStdLib::Menu::MenuEntry(L"Check Number", CheckNumber));
* 
* MainMenu.AddMenuEntry(new NosStdLib::Menu::MenuEntry(L"Another Menu", &SecondaryMenu));
* 
* MainMenu.AddMenuEntry(new NosStdLib::Menu::MenuEntry(L"========== Boolean =========="));
* 
* MainMenu.AddMenuEntry(new NosStdLib::Menu::MenuEntry(L"Toggle", &SomeBool));
* MainMenu.AddMenuEntry(new NosStdLib::Menu::MenuEntry(L"Check Bool", CheckBool));
* 
* MainMenu.AddMenuEntry(new NosStdLib::Menu::MenuEntry(L"========== Integer =========="));
* 
* MainMenu.AddMenuEntry(new NosStdLib::Menu::MenuEntry(L"Number", &number));
* MainMenu.AddMenuEntry(new NosStdLib::Menu::MenuEntry(L"Check Number", CheckNumber));
* 
* MainMenu.StartMenu();
* 
*/

/* LOADING SCREEN TEST
NosStdLib::LoadingScreen::InitilizeFont();


std::wstring splash = LR"(
                      ████████                ███████
                    ▄██▀    ▀██▄ ▄███████▄  ███▀   ▀████████▄
          ▄███████████▌      ██████     ▀█████       ███     ▀▀███▄
     ▄██▀▀         ██▌        ████       ████▌       ███           ▀▀███▄
   ██▀            ███         ███▌       ▐███        ▐██▄               ▀▀███▄
 ██▀       ███    ███         ███▌       ▐███        ▐████▀                  ▀██
██▌       ▀███▄▄▄▄███         ███        ▐███        ████▌                     ██
██▌               ██▌         ███        ▐███        ███▌          ████▄▄     ▄██
▀██▄              ██▌         ███        ▐███        ███          ███    ▀█████▀
  ▀██████████████▄███         ███        ████       ███          ███
    ██▀       ████▀██         ███        ▐██▌      ▐██▌          ██▌
   ███             ██▌        ██▌         ██       ███▌         ███
   ███             ▐██                            █████▄       ███
    ▀██▄▄       ▄▄▄████▄                         ███   ▀███▄▄███▀
       ▀▀▀███▀▀▀▀    ▀██▄         ▄██▄         ▄██▀
                       ▀███▄▄▄▄▄███▀████▄▄▄▄▄███▀
                           ▀▀▀▀▀        ▀▀▀▀▀)";

std::wstring SomeVar(L"Some Text");

NosStdLib::LoadingScreen LC(NosStdLib::LoadingScreen::LoadType::Known, splash);
LC.StartLoading(&LongFunctionTing, &SomeVar);

wprintf(L"Press any button to continue"); getchar();
NosStdLib::LoadingScreen::TerminateFont();

*/