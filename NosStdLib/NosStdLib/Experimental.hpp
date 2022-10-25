#ifndef _EXPERIMENTAL_HPP_
#define _EXPERIMENTAL_HPP_

#include <Windows.h>
#include <string>
#include <codecvt>
#include "Global.hpp"

namespace NosStdLib
{
    /// <summary>
    /// namespace contains items which are experimental (require more testing)
    /// </summary>
    namespace Experimental
    {
        /// <summary>
        /// function to convert global wstring to string
        /// </summary>
        /// <param name="globalWString">- the text to convert</param>
        /// <param name="output">- the output for overloads</param>
        /// <returns>pointer to output string</returns>
        std::string* ConvertGlobal(const std::wstring& globalWString, std::string* output)
        {
            *output = NosStdLib::Global::String::ToString(globalWString);
            return output;
        }

        /// <summary>
        /// function to convert global wstring to wstring
        /// </summary>
        /// <param name="globalWString">- the text to convert</param>
        /// <param name="output">- the output for overloads</param>
        /// <returns>pointer to output wstring</returns>
        std::wstring* ConvertGlobal(const std::wstring& globalWString, std::wstring* output)
        {
            *output = globalWString;
            return output;
        }
    }

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
			return std::wstring(max(position - 1, 0), L' ') + L"|" + std::wstring(max(columnCount - position, 0), L' ');
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