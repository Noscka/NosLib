#include "NosStdLib/Console.hpp"
#include "NosStdLib/String.hpp"
#include "NosStdLib/DynamicArray.hpp"

#include <Windows.h>
#include <iostream>
#include <io.h>
#include <fcntl.h>
#include <iostream> 
#include <cstdio> 
#include <conio.h>

/* TODO: Figure out if it is worth it to change calling convention from default (__cdelc) to __fastcall */

int main()
{
    NosStdLib::Console::InitializeModifiers::EnableUnicode();
    NosStdLib::Console::InitializeModifiers::EnableANSI();
    NosStdLib::Console::InitializeModifiers::BeatifyConsole<wchar_t>(L"Dynamic Array Development");
    NosStdLib::Console::InitializeModifiers::InitializeEventHandler();

    NosStdLib::DynamicArray<int> testingDynamicArray;

    for (int i = 0; i <= 30; i++)
    {
        testingDynamicArray.Append((i+70));
    }

    std::wcout << testingDynamicArray << std::endl;

    wprintf(L"Press any button to continue"); _getch();
    return 0;
}