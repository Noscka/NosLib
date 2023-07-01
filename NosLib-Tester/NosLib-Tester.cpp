#include "NosLib/Console.hpp"
#include "NosLib/DynamicMenuSystem.hpp"

#include <Windows.h>
#include <conio.h>

/* TODO: Figure out if it is worth it to change calling convention from default (__cdelc) to __fastcall */

int main()
{
    NosLib::Console::InitializeModifiers::EnableUnicode();
    NosLib::Console::InitializeModifiers::EnableANSI();
    //NosLib::Console::InitializeModifiers::BeatifyConsole<wchar_t>(L"Dynamic Array Updating");
    NosLib::Console::InitializeModifiers::InitializeEventHandler();

	wprintf(L"Press any button to continue"); _getch();

	SCROLLINFO info;
	info.cbSize = sizeof(SCROLLINFO);
	info.fMask = SIF_ALL;

	GetScrollInfo(GetConsoleWindow(), SB_VERT, &info);

	wprintf(L"\033[%d;0f", (SHORT)(info.nPos));
	wprintf(L"Virtual\n");
	wprintf(L"Max		- %d\n", info.nMax);
	wprintf(L"Min		- %d\n", info.nMin);
	wprintf(L"Page		- %d\n", info.nPage);
	wprintf(L"Pos		- %d\n", info.nPos);
	wprintf(L"TrackPos	- %d\n", info.nTrackPos);

	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), {0, (SHORT)(info.nPos)});
	wprintf(L"API\n");
	wprintf(L"Max		- %d\n", info.nMax);
	wprintf(L"Min		- %d\n", info.nMin);
	wprintf(L"Page		- %d\n", info.nPage);
	wprintf(L"Pos		- %d\n", info.nPos);
	wprintf(L"TrackPos	- %d\n", info.nTrackPos);

	/*int variable1 = 0;

	NosLib::Menu::DynamicMenu mainMenu(L"Basic Menu");

	NosLib::Menu::DynamicMenu secondMenu(L"Basic Menu");

	mainMenu.AddMenuEntry(new NosLib::Menu::MenuEntry<NosLib::Menu::DynamicMenu>(L"second menu", &secondMenu));

	for (int i = 0; i <= 40; i++)
	{
		mainMenu.AddMenuEntry(new NosLib::Menu::MenuEntry<int>(std::format(L"int {}", i), &variable1));
	}

	mainMenu.StartMenu();*/

    wprintf(L"Press any button to continue"); _getch();
    return 0;
}