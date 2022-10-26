#ifndef _EXPERIMENTAL_HPP_
#define _EXPERIMENTAL_HPP_

#include "TextColor.hpp"
#include <Windows.h>
#include <string>
#include <codecvt>
#include "Global.hpp"

namespace NosStdLib
{
	/// <summary>
	/// namespace which contains functions and classes which get used for testing (this namespace will most likely not have comments)
	/// </summary>
    namespace TestEnv
    {
        LPPOINT GetCaretPositionReturn()
        {
            LPPOINT point = new POINT();
            GetCaretPos(point);
            return point;
        }

        std::wstring DrawSquare(int position, int columnCount)
        {
            std::wstring ANSIEscapeCodeStart = NosStdLib::TextColor::MakeANSICode<wchar_t>(NosStdLib::TextColor::NosRGB(20, 180, 170));
            std::wstring LeftPadding = std::wstring(max(position - 1, 0), L' ');
            std::wstring BoxCharacter = L"|";
            std::wstring RightPadding = std::wstring(max(columnCount - position, 0), L' ');
            std::wstring ANSIEscapeCodeEnd = L"\033[0m";

            return  ANSIEscapeCodeStart + LeftPadding + BoxCharacter + RightPadding + ANSIEscapeCodeEnd;
        }

        void IterateSquare(int sleepSpeed = 15)
        {
            NosStdLib::Global::Console::ShowCaret(false);

            HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
            CONSOLE_SCREEN_BUFFER_INFO csbi;
            GetConsoleScreenBufferInfo(consoleHandle, &csbi);
            int columns = csbi.srWindow.Right - csbi.srWindow.Left + 1;
            int rows = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;

            for (int i = 0; i <= columns; i++)
            {
                wprintf((NosStdLib::TestEnv::DrawSquare(i, columns) + L'\n').c_str());
                wprintf((NosStdLib::TestEnv::DrawSquare(i, columns) + L'\n').c_str());
                wprintf((NosStdLib::TestEnv::DrawSquare(i, columns) + L'\n').c_str());
                wprintf((NosStdLib::TestEnv::DrawSquare(i, columns) + L'\n').c_str());
                SetConsoleCursorPosition(consoleHandle, { 0, 0 });
                Sleep(sleepSpeed);
            }

            for (int i = columns; i > 0; i--)
            {
                wprintf((NosStdLib::TestEnv::DrawSquare(i, columns) + L'\n').c_str());
                wprintf((NosStdLib::TestEnv::DrawSquare(i, columns) + L'\n').c_str());
                wprintf((NosStdLib::TestEnv::DrawSquare(i, columns) + L'\n').c_str());
                wprintf((NosStdLib::TestEnv::DrawSquare(i, columns) + L'\n').c_str());
                SetConsoleCursorPosition(consoleHandle, { 0, 0 });
                Sleep(sleepSpeed);
            }

            NosStdLib::Global::Console::ShowCaret(true);
        }
    }
}

#endif