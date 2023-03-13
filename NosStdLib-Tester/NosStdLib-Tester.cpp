﻿#include "NosStdLib/Console.hpp"
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

    MSG msg;
    
    while (true)
    {
        std::wcout << HIWORD(GetQueueStatus(QS_ALLEVENTS)) << std::endl;
        if (HIWORD(GetQueueStatus(QS_ALLEVENTS)))
        {
            PeekMessage(&msg, NULL, 0, 0, PM_REMOVE);
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }


    //while (true)
    //{
    //    switch (MsgWaitForMultipleObjects(1, handleArray, FALSE, INFINITE, QS_MOUSE))
    //    {
    //    case WAIT_OBJECT_0:
    //        wprintf(L"Nothing To Do\n");
    //        break;
    //    case WAIT_OBJECT_0 + 1:
    //        while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
    //        {
    //            // TODO:  must handle WM_QUIT; see Raymond's blog for details
    //            TranslateMessage(&msg);
    //            DispatchMessage(&msg);
    //        }
    //        break;
    //    default:
    //        return -1; // unexpected failure
    //    }
    //    
    //}

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