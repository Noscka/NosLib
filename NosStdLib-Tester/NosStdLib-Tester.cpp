#include "NosStdLib/Console.hpp"
#include "NosStdLib/Chat.hpp"

#include "NosStdLib/Definitions.hpp"

#include <Windows.h>
#include <conio.h>

/* TODO: Figure out if it is worth it to change calling convention from default (__cdelc) to __fastcall */

std::wstring GetUserInput() /* TESTING */
{
    std::wstring output;
    unsigned int negativeOffset = 0;

    while (true)
    {
        int ch = _getch();

        if (ch == NosStdLib::Definitions::ENTER)
        {
            wprintf(L"\n");
            break; /* Finish gathering */
        }
        else if (ch == NosStdLib::Definitions::BACKSPACE)
        {
			// Coord for backspace cursor position editing
			COORD NewCoord;
            CONSOLE_SCREEN_BUFFER_INFO consoleInfo;

			if (!output.empty())
			{
                NosStdLib::Console::GetConsoleSize(GetStdHandle(STD_OUTPUT_HANDLE), &consoleInfo);

				NewCoord = {(SHORT)(consoleInfo.dwCursorPosition.X - 1), consoleInfo.dwCursorPosition.Y}; // create new coord with x-1 and same y
				SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), NewCoord); // use new coord
				wprintf(L" "); // delete character
				SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), NewCoord);
                output.pop_back();
			}

            continue; /* skip printing and adding character to string */
        }
        else if (!(ch && ch != 224))
        {
            int exCh = _getch();

            switch (exCh)
            {
            case NosStdLib::Definitions::ARROW_LEFT:
                if (negativeOffset < output.size()) /* only if less then the size of the current string */
                {
                    negativeOffset++; /* increase backwards offset */
                }
                continue; /* Don't process character */

            case NosStdLib::Definitions::ARROW_RIGHT:
				if (negativeOffset > 0) /* only if offset is more then 0 */
                {
                    negativeOffset--;
                }
				continue; /* Don't process character */
            }
        }

        wprintf(std::format(L"{}", (wchar_t)ch).c_str());
        output.insert(output.end() - negativeOffset, (wchar_t)ch);

    }

    return output;
}

int main()
{
    NosStdLib::Console::InitializeModifiers::EnableUnicode();
    NosStdLib::Console::InitializeModifiers::EnableANSI();
    NosStdLib::Console::InitializeModifiers::BeatifyConsole<wchar_t>(L"Drag and Drop Testing");
    NosStdLib::Console::InitializeModifiers::InitializeEventHandler();

    std::wstring output;

    wprintf(L"quick input: ");
    getline(std::wcin, output);
	wprintf(L"%s\n", output.c_str());
	wprintf(L"%zd\n", output.size());

	wprintf(L"quick input: ");
    output = GetUserInput();
    wprintf(L"%s\n", output.c_str());
    wprintf(L"%zd\n", output.size());

    wprintf(L"Press any button to continue"); _getch();
	return 0;

/* NOT GONNA RUN */

    NosStdLib::Chat::DynamicChat mainChat;

    mainChat.StartChat();

    wprintf(L"Press any button to continue"); _getch();
    return 0;
}