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

//bool SomeBool = false;
//int number = 1;
//void CheckBool()
//{
//    wprintf((SomeBool ? L"true\n" : L"false\n"));
//    wprintf(L"Press any button to continue"); _getch();
//    return;
//}
//void CheckNumber()
//{
//    wprintf((std::to_wstring(number) + L"\n").c_str());
//    wprintf(L"Press any button to continue"); _getch();
//    return;
//}
//void SomeFunction(int* param1, int* param2)
//{
//    std::wcout << L"Param1: " << *param1 << L" | Param2: " << *param2 << std::endl;
//    wprintf(L"Press any button to continue"); _getch();
//    return;
//}
//void PrintingFunction(std::wstring input)
//{
//    wprintf(input.c_str());
//}


std::condition_variable cv{};
std::mutex mtx;
std::queue<int> char_queue{};

void WaitForInput_Thread()
{
    int ch;
    int exCh;
    while (true)
    {
        ch = _getch();
        std::unique_lock<std::mutex> lock{mtx};
        char_queue.push(ch);
        if (!(ch && ch != 224))
        {
            exCh = _getch();
            char_queue.push(exCh);
        }
        cv.notify_all();
    }
    return;
}

int main()
{
    NosStdLib::Console::InitializeModifiers::EnableUnicode();
    NosStdLib::Console::InitializeModifiers::EnableANSI();
    NosStdLib::Console::InitializeModifiers::BeatifyConsole<wchar_t>(L"Mouse Tracking");
    NosStdLib::Console::InitializeModifiers::InitializeEventHandler();

    NosStdLib::MouseTracking::InitializeMouseTracking();

    MSG msg;

    std::thread inputListenThread{[]() {WaitForInput_Thread(); }}; /* start waiting for input thread */

    int ch, exCh;

    while (true)
    {
        std::unique_lock<std::mutex> lock{mtx};
        switch (MsgWaitForMultipleObjects(0, NULL, FALSE, 5, QS_ALLINPUT))
        {
        case WAIT_OBJECT_0:
            PeekMessage(&msg, NULL, 0, 0, PM_REMOVE);
            TranslateMessage(&msg);
            DispatchMessage(&msg); /* break removed on purpose to also run timout code */

        case WAIT_TIMEOUT:
            cv.wait_for(lock, std::chrono::system_clock::duration(std::chrono::milliseconds(2)), []() {return !char_queue.empty(); });
            if (!char_queue.empty())
            {
                ch = char_queue.front();
                char_queue.pop();
                if (!(ch && ch != 224))
                {
                    exCh = char_queue.front();
                    char_queue.pop();
                }

                wprintf(std::format(L"First Character: {}\n", ch).c_str());
                wprintf(std::format(L"Second Character: {}\n", exCh).c_str());
            }
            break;
        }
    }

    inputListenThread.join();

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

    wprintf(L"Press any button to continue"); _getch();
    return 0;
}