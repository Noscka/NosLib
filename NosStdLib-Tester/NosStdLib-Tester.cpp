#include "NosStdLib/Console.hpp"
#include "NosStdLib/Chat.hpp"

#include "NosStdLib/Definitions.hpp"

#include <Windows.h>
#include <conio.h>

/* TODO: Figure out if it is worth it to change calling convention from default (__cdelc) to __fastcall */

void AddMessageLoop(NosStdLib::Chat::DynamicChat* chatPointer)
{
	int counter = 0;
	while (true)
	{
		counter++;
		chatPointer->AddMessage(std::format(L"{}) Some Random Message", counter));
		Sleep(100);
	}
}

void printMessageAgain(const std::wstring& message)
{
	wprintf(message.c_str());
	Sleep(1000);
	return;
}

int main()
{
    NosStdLib::Console::InitializeModifiers::EnableUnicode();
    NosStdLib::Console::InitializeModifiers::EnableANSI();
    NosStdLib::Console::InitializeModifiers::BeatifyConsole<wchar_t>(L"Drag and Drop Testing");
    NosStdLib::Console::InitializeModifiers::InitializeEventHandler();

	NosStdLib::Console::ShowCaret(false);

	NosStdLib::Chat::DynamicChat mainChat;

	mainChat.OnMessageReceived.AssignEventFunction(&printMessageAgain, L"");

	std::thread messageReceiveThread(AddMessageLoop, &mainChat);

	mainChat.StartChat();

    wprintf(L"Press any button to continue"); _getch();
    return 0;
}