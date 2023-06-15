#include "NosStdLib/Console.hpp"
#include "NosStdLib/Chat.hpp"

#include "NosStdLib/Definitions.hpp"

#include <Windows.h>
#include <conio.h>

/* TODO: Figure out if it is worth it to change calling convention from default (__cdelc) to __fastcall */

std::wstring GetUserInput() /* TESTING */
{
    std::wstring output;

    while (true)
    {
        int ch = _getch();

        if (ch == NosStdLib::Definitions::ENTER)
        {
            break; /* Finish gathering */
        }
        else if (!(ch && ch != 224))
        {
            int exCh = _getch();

            switch (exCh)
            {
            case NosStdLib::Definitions::ARROW_LEFT:
                break;
            case NosStdLib::Definitions::ARROW_RIGHT:
                break;
            }
        }

        output.push_back(ch);
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
    wprintf(std::format(L"{}\n", output).c_str());
    wprintf(std::format(L"size: {}\n", output.size()).c_str());

    wprintf(L"Press any button to continue"); _getch();
	return 0;

/* NOT GONNA RUN */

    NosStdLib::Chat::DynamicChat mainChat;

    mainChat.StartChat();

    wprintf(L"Press any button to continue"); _getch();
    return 0;
}