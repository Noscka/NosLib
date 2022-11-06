#include <NosStdLib/DynamicArray.hpp>
#include <NosStdLib/Global.hpp>

#include <Windows.h>
#include <iostream>
#include <io.h>
#include <fcntl.h>
#include <iostream> 
#include <cstdio> 
#include <conio.h>

int main()
{
    NosStdLib::Global::Console::InitializeModifiers::EnableUnicode();
    NosStdLib::Global::Console::InitializeModifiers::EnableANSI();

    NosStdLib::DynamicArray<wchar_t> dArray;

    dArray.Append('a');
    dArray.Append('b');
    dArray.Append('c');

    std::wcout << dArray << std::endl;

    wprintf(L"Press any button to continue"); _getch();
    return 0;
}

/* ANSI TEST
std::wcout << L"🯰" << std::endl;
std::wcout << L"\033[48;2;255;255;255;38;2;20;120;20m" << L"text 1" << L"\033[0m" << std::endl;
std::wcout << L"\033[38;5;214;48;5;124m" << L"text 2" << L"\033[0m"<< std::endl;
std::wcout << L"\033[5m" << L"text 2" << L"\033[0m"<< std::endl;
std::wcout << NosStdLib::TextColor::MakeANSICode<wchar_t>(NosStdLib::TextColor::NosRGB(140, 20, 30)) << L"abc" << L"\033[0m" << std::endl;
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

====MAIN====
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
*/

/* LOADING SCREEN TEST
void LongFunctionTing(NosStdLib::LoadingScreen* Object, std::wstring* argue)
{
    int total = 1000;
    uint8_t R = 255, G = 0, B = 0;
    uint8_t changeValue = 1;
    for (int i = 0; i <= total; i++)
    {
        if (R > 0 && B == 0)
        {
            R -= changeValue;
            G += changeValue;
        }
        if (G > 0 && R == 0)
        {
            G -= changeValue;
            B += changeValue;
        }
        if (B > 0 && G == 0)
        {
            R += changeValue;
            B -= changeValue;
        }

        Object->UpdateKnownProgressBar((float)i / (float)total, std::vformat((NosStdLib::TextColor::MakeANSICode<wchar_t>(NosStdLib::TextColor::NosRGB(R, G, B)) + L"\n{}\nTesting Status\nExtra Line,\n Innit\n{}" + L"\033[0m"), std::make_wformat_args((float)i / (float)total, NosStdLib::FileManagement::GetFileExtension<wchar_t>(L"abc.txt"), true)));
        Sleep(1);
    }
    *argue = L"Completed ting innit fam";
}

void ChangingProgressFunction(NosStdLib::LoadingScreen* Object, std::wstring* argue)
{
    int total = 1000;
    Object->UpdateKnownProgressBar(0.0 / (float)total, L"Testing Status\nExtra Line,\n Innit", true);
    Sleep(1000);
    Object->UpdateKnownProgressBar(250.0 / (float)total, L"Testing Status\nExtra Line,\n Innit", true);
    Sleep(1000);
    Object->UpdateKnownProgressBar(100.0 / (float)total, L"Testing Status\nExtra Line,\n Innit", true);
    Sleep(1000);

    *argue = L"Completed ting innit fam";
}

====MAIN====
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
                           ▀▀▀▀▀        ▀▀▀▀▀                                    )";

std::wstring SomeVar(L"Some Text");

NosStdLib::LoadingScreen LC(NosStdLib::LoadingScreen::LoadType::Known, splash);
LC.StartLoading(&LongFunctionTing, &SomeVar);

wprintf(L"Press any button to continue"); getchar();
NosStdLib::LoadingScreen::TerminateFont();
*/