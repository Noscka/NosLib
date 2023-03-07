#include "NosStdLib/Console.hpp"
#include "NosStdLib/String.hpp"
#include "NosStdLib/DynamicMenuSystem.hpp"
#include "NosStdLib/MouseTracking/MouseTracking.hpp"

#include <Windows.h>
#include <iostream>
#include <io.h>
#include <fcntl.h>
#include <iostream> 
#include <cstdio> 
#include <conio.h>

/* TODO: Figure out if it is worth it to change calling convention from default (__cdelc) to __fastcall */

#include <iostream>
#include <string>
#include <chrono>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <queue>

std::condition_variable cv{};
std::mutex mtx;
std::queue<wchar_t> char_queue{};
bool quit{ false };

void add_chars_to_queue()
{
    wchar_t c{};
    for (;;)
    {
        c = static_cast<wchar_t>(_getch());
        if (!std::wcin)
        {
            std::unique_lock<std::mutex> lck{ mtx };
            quit = true;
            cv.notify_all();
            return;
        }
        if (c == L'q' || c == L'Q')
        {
            std::unique_lock<std::mutex> lck{ mtx };
            quit = true;
            char_queue.push(c);
            cv.notify_all();
            return;
        }
        if (c == '\n')
            continue;
        std::unique_lock<std::mutex> lck{ mtx };
        char_queue.push(c);
        cv.notify_all();
    }
}


void get_key_or_wait(std::chrono::system_clock::duration d)
{
    std::unique_lock<std::mutex> lck{ mtx };
    for (int i{ 10 }; i > 0; --i)
    {

        cv.wait_for(lck, d / 10., []() { return quit || !char_queue.empty(); });
        if (!char_queue.empty())
        {
            break;
        }
        if (quit)
        {
            wprintf(L"Quitting.\n");
            return;
        }
        std::wcout << L"Countdown at " << i << '\n';
    }
    if (!char_queue.empty())
    {
        wprintf(std::format(L"Obtained a character from the stream before the timer ran out. Character was: {}\n", char_queue.front()).c_str());
        char_queue.pop();
    }
    else
    {
        wprintf(L"Timer ran out.\n");
    }

    return;
}

void PrintingFunction(std::wstring input)
{
    wprintf(input.c_str());
}

int main()
{
    NosStdLib::Console::InitializeModifiers::EnableUnicode();
    NosStdLib::Console::InitializeModifiers::EnableANSI();
    NosStdLib::Console::InitializeModifiers::BeatifyConsole<wchar_t>(L"Mouse Tracking");
    NosStdLib::Console::InitializeModifiers::InitializeEventHandler();

    NosStdLib::Clickable::Button button1(L"button 1", NosStdLib::Dimention::DimentionsD2(1, 1, 10, 3));
    NosStdLib::Clickable::Button button2(L"button 2", NosStdLib::Dimention::DimentionsD2(20, 3, 29, 5));

    NosStdLib::Clickable::Button::PrintAllButtons();

    button1.OnClick = new NosStdLib::Event(new NosStdLib::Functional::FunctionStore<void(std::wstring), std::wstring>(&PrintingFunction, L"Clicked button 1\n"));

    button2.OnClick = new NosStdLib::Event(new NosStdLib::Functional::FunctionStore<void(std::wstring), std::wstring>(&PrintingFunction, L"Clicked button 2\n"));

    NosStdLib::MouseTracking::InitializeMouseTracking();


    std::thread get_chars{ []() { add_chars_to_queue(); } };

    wprintf(L"Type q to exit.\n");
    MSG msg;
    //while (true)
    while(GetMessage(&msg, 0, 0, 0))
    {
        {
            std::lock_guard<std::mutex> lck{ mtx };
            if (quit)
                break;
        }
        wprintf(L"Waiting for key press followed by <enter>.\n");
        get_key_or_wait(std::chrono::seconds(10));
    }

    get_chars.join();

    wprintf(L"Press any button to continue"); _getch();
    return 0;
}

/* ANSI TEST
std::wcout << L"🯰" << std::endl;
std::wcout << NosStdLib::TextColor::NosRGB(255, 255, 255).MakeANSICode<wchar_t>(false) << NosStdLib::TextColor::NosRGB(20, 120, 20).MakeANSICode<wchar_t>() << L"text 1" << L"\033[0m" << std::endl;
std::wcout << NosStdLib::TextColor::NosRGB(214, 48, 124).MakeANSICode<wchar_t>() << L"text 2" << L"\033[0m" << std::endl;
std::wcout << NosStdLib::TextColor::NosRGB(140, 20, 30).MakeANSICode<wchar_t>() << L"abc" << L"\033[0m" << std::endl;
*/

/* MENU TEST
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

====MAIN====
NosStdLib::Console::InitializeModifiers::EnableUnicode();
NosStdLib::Console::InitializeModifiers::EnableANSI();

NosStdLib::Menu::DynamicMenu MainMenu(L"Main Menu", true, true, true);
NosStdLib::Menu::DynamicMenu SecondaryMenu(L"Second Menu", true, true, true);

SecondaryMenu.AddMenuEntry(new NosStdLib::Menu::MenuEntry(L"Number", &number));
SecondaryMenu.AddMenuEntry(new NosStdLib::Menu::MenuEntry(L"Check Number", new NosStdLib::Functional::FunctionStore(&CheckNumber)));

int param1, param2;

SecondaryMenu.AddMenuEntry(new NosStdLib::Menu::MenuEntry(L"param1", &param1));
SecondaryMenu.AddMenuEntry(new NosStdLib::Menu::MenuEntry(L"param2", &param2));

SecondaryMenu.AddMenuEntry(new NosStdLib::Menu::MenuEntry(L"Run Function", new NosStdLib::Functional::FunctionStore(&SomeFunction, &param1, &param2)));

MainMenu.AddMenuEntry(new NosStdLib::Menu::MenuEntry(L"Another Menu", &SecondaryMenu));

MainMenu.AddMenuEntry(new NosStdLib::Menu::MenuEntry(L"Toggle", &SomeBool));
MainMenu.AddMenuEntry(new NosStdLib::Menu::MenuEntry(L"Check Bool", new NosStdLib::Functional::FunctionStore(&CheckBool)));

MainMenu.AddMenuEntry(new NosStdLib::Menu::MenuEntry(L"Number", &number));
MainMenu.AddMenuEntry(new NosStdLib::Menu::MenuEntry(L"Check Number", new NosStdLib::Functional::FunctionStore(&CheckNumber)));

MainMenu.StartMenu();
wprintf(L"Press any button to continue"); _getch();
return 0;
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

wprintf(L"Press any button to continue"); _getch();
NosStdLib::LoadingScreen::TerminateFont();
*/