#include "NosStdLib/Console.hpp"
#include "NosStdLib/String.hpp"
#include "NosStdLib/DynamicMenuSystem.hpp"
#include "NosStdLib/DynamicLoadingScreen.hpp"

#include <Windows.h>
#include <iostream>
#include <io.h>
#include <fcntl.h>
#include <iostream> 
#include <cstdio> 
#include <conio.h>

/* TODO: Figure out if it is worth it to change calling convention from default (__cdelc) to __fastcall */

void PrintIntValue(int* pointerToInt)
{
    wprintf(L"%d\n", *pointerToInt);
    wprintf(L"Press any button to continue"); _getch();
    return;
}

void WaitingFunction(NosStdLib::LoadingScreen* loadingCallerPointer)
{
    for (int i = 0; i < 100; i++)
    {
        loadingCallerPointer->UpdateKnownProgressBar(i/100, std::format(L"{} done", i), true);
        Sleep(10);
    }
    
    return;
}

int main()
{
    NosStdLib::Console::InitializeModifiers::EnableUnicode();
    NosStdLib::Console::InitializeModifiers::EnableANSI();
    NosStdLib::Console::InitializeModifiers::BeatifyConsole<wchar_t>(L"Dynamic Array Development");
    NosStdLib::Console::InitializeModifiers::InitializeEventHandler();

    namespace menu = NosStdLib::Menu;
    using loadingScreen = NosStdLib::LoadingScreen;

    loadingScreen::InitilizeFont();

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

    NosStdLib::LoadingScreen loadingScreenTest(NosStdLib::LoadingScreen::LoadType::Known, splash);
    loadingScreenTest.StartLoading(&WaitingFunction);

    loadingScreen::TerminateFont();

    int intPointer = 0;

    menu::DynamicMenu mainMenu(L"Some Title", true, true, true);
    mainMenu.AddMenuEntry(new menu::MenuEntry<int>(L"Interger Ting", &intPointer));

    mainMenu.AddMenuEntry(new menu::MenuEntry(L"Print Function", new NosStdLib::Functional::FunctionStore(&PrintIntValue, &intPointer)));


    mainMenu.StartMenu();

    wprintf(L"Press any button to continue"); _getch();
    return 0;
}