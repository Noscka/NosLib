#include "NosStdLib/Console.hpp"
#include "NosStdLib/Chat.hpp"

#include <Windows.h>
#include <conio.h>

/* TODO: Figure out if it is worth it to change calling convention from default (__cdelc) to __fastcall */


int main()
{
    NosStdLib::Console::InitializeModifiers::EnableUnicode();
    NosStdLib::Console::InitializeModifiers::EnableANSI();
    NosStdLib::Console::InitializeModifiers::BeatifyConsole<wchar_t>(L"Drag and Drop Testing");
    NosStdLib::Console::InitializeModifiers::InitializeEventHandler();

    std::wstring output;

    wprintf(L"quick input: ");
    getline(std::wcin, output);
    wprintf(output.c_str());

    wprintf(L"Press any button to continue"); _getch();
	return 0;

/* NOT GONNA RUN */

    NosStdLib::Chat::DynamicChat mainChat;

    mainChat.StartChat();

    wprintf(L"Press any button to continue"); _getch();
    return 0;
}