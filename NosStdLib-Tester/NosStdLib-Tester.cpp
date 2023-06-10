#include "NosStdLib/Console.hpp"
#include "NosStdLib/Chat.hpp"

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
    NosStdLib::Console::InitializeModifiers::BeatifyConsole<wchar_t>(L"Drag and Drop Testing");
    NosStdLib::Console::InitializeModifiers::InitializeEventHandler();

    NosStdLib::Chat::DynamicChat mainChat;

    mainChat.StartChat();

    wprintf(L"Press any button to continue"); _getch();
    return 0;
}