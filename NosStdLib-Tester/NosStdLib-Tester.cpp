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

void add_chars_to_queue()
{
    wchar_t c{};
    while(true)
    {
        c = _getch();
        std::unique_lock<std::mutex> lck{mtx};
        char_queue.push(c);
        cv.notify_all();
    }
}

int main()
{
    NosStdLib::Console::InitializeModifiers::EnableUnicode();
    NosStdLib::Console::InitializeModifiers::EnableANSI();
    NosStdLib::Console::InitializeModifiers::BeatifyConsole<wchar_t>(L"Mouse Tracking");
    NosStdLib::Console::InitializeModifiers::InitializeEventHandler();

    NosStdLib::MouseTracking::InitializeMouseTracking();

    std::thread get_chars{[]() {add_chars_to_queue(); }};
    MSG msg;

    while (true)
    {
        std::unique_lock<std::mutex> lck{mtx};
        switch (MsgWaitForMultipleObjects(0, NULL, FALSE, 5, QS_ALLINPUT))
        {
        case WAIT_OBJECT_0:
            PeekMessage(&msg, NULL, 0, 0, PM_REMOVE);
            TranslateMessage(&msg);
            DispatchMessage(&msg);
            break;

        case WAIT_TIMEOUT:
            cv.wait_for(lck, std::chrono::system_clock::duration(std::chrono::milliseconds(2)), []() {return !char_queue.empty(); });
            if (!char_queue.empty())
            {
                wprintf(std::format(L"Obtained a character from the stream before the timer ran out. Character was: {}\n", char_queue.front()).c_str());
                char_queue.pop();
            }
            break;
        }
    }

    get_chars.join();

    /*MSG msg;

    while (true)
    {
        switch (MsgWaitForMultipleObjects(0, NULL, FALSE, 5, QS_ALLINPUT))
        {
        case WAIT_OBJECT_0:
            PeekMessage(&msg, NULL, 0, 0, PM_REMOVE);
            TranslateMessage(&msg);
            DispatchMessage(&msg);
            break;

        case WAIT_TIMEOUT:
            wprintf(L"timed out\n");
            break;
        }
    }*/

    wprintf(L"Press any button to continue"); _getch();
    return 0;

    //NosStdLib::MouseTracking::InitializeMouseTracking();

    //NosStdLib::Menu::DynamicMenu MainMenu(L"Main Menu", true, true, true);
    //NosStdLib::Menu::DynamicMenu SecondaryMenu(L"Second Menu", true, true, true);

    //SecondaryMenu.AddMenuEntry(new NosStdLib::Menu::MenuEntry(L"Number", &number));
    //SecondaryMenu.AddMenuEntry(new NosStdLib::Menu::MenuEntry(L"Check Number", new NosStdLib::Functional::FunctionStore(&CheckNumber)));

    //int param1 = 0,
    //    param2 = 10;

    //SecondaryMenu.AddMenuEntry(new NosStdLib::Menu::MenuEntry(L"param1", &param1));
    //SecondaryMenu.AddMenuEntry(new NosStdLib::Menu::MenuEntry(L"param2", &param2));

    //SecondaryMenu.AddMenuEntry(new NosStdLib::Menu::MenuEntry(L"Run Function", new NosStdLib::Functional::FunctionStore(&SomeFunction, &param1, &param2)));

    //MainMenu.AddMenuEntry(new NosStdLib::Menu::MenuEntry(L"Another Menu", &SecondaryMenu));

    //MainMenu.AddMenuEntry(new NosStdLib::Menu::MenuEntry(L"Toggle", &SomeBool));
    //MainMenu.AddMenuEntry(new NosStdLib::Menu::MenuEntry(L"Check Bool", new NosStdLib::Functional::FunctionStore(&CheckBool)));

    //MainMenu.AddMenuEntry(new NosStdLib::Menu::MenuEntry(L"Number", &number));
    //MainMenu.AddMenuEntry(new NosStdLib::Menu::MenuEntry(L"Check Number", new NosStdLib::Functional::FunctionStore(&CheckNumber)));

    //MainMenu.StartMenu();

    //wprintf(L"Press any button to continue"); _getch();
    //return 0;
}