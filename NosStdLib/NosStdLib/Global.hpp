#ifndef _GLOBAL_HPP_
#define _GLOBAL_HPP_

#include <string>
#include <stringapiset.h>
#include <Windows.h>

/*
Contains:

String
    - to_wstring
    - to_string
*/


namespace NosStdLib
{
    namespace Global
    {
        namespace String
        {
            /// <summary>
            /// Converts string to wstring
            /// </summary>
            /// <param name="str">- string for converting</param>
            /// <returns>wstring version of the string</returns>
            std::wstring ToWstring(const std::string& str)
            {
                if (str.empty()) return std::wstring();
                int size_needed = MultiByteToWideChar(CP_UTF8, 0, &str[0], (int)str.size(), NULL, 0);
                std::wstring wstrTo(size_needed, 0);
                MultiByteToWideChar(CP_UTF8, 0, &str[0], (int)str.size(), &wstrTo[0], size_needed);
                return wstrTo;
            }

            /// <summary>
            /// Converts wstring to string
            /// </summary>
            /// <param name="wstr">- wstring for converting</param>
            /// <returns>string version of the string</returns>
            std::string ToString(const std::wstring& wstr)
            {
                if (wstr.empty()) return std::string();
                int size_needed = WideCharToMultiByte(CP_UTF8, 0, &wstr[0], (int)wstr.size(), NULL, 0, NULL, NULL);
                std::string strTo(size_needed, 0);
                WideCharToMultiByte(CP_UTF8, 0, &wstr[0], (int)wstr.size(), &strTo[0], size_needed, NULL, NULL);
                return strTo;
            }
        }

        namespace Console
        {
            void ClearRange(int position, int range, wchar_t fillChar = L' ')
            {
                CONSOLE_SCREEN_BUFFER_INFO csbi;
                HANDLE ConsoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);

                COORD tl = { 0, (SHORT)(position) };
                GetConsoleScreenBufferInfo(ConsoleHandle, &csbi);
                DWORD written, cells = csbi.dwSize.X * (1 + range);
                FillConsoleOutputCharacter(ConsoleHandle, fillChar, cells, tl, &written);
                FillConsoleOutputAttribute(ConsoleHandle, csbi.wAttributes, cells, tl, &written);
                SetConsoleCursorPosition(ConsoleHandle, tl);
            }

            void ClearScreen(wchar_t fillChar = L' ')
            {
                CONSOLE_SCREEN_BUFFER_INFO csbi;
                HANDLE ConsoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
                GetConsoleScreenBufferInfo(ConsoleHandle, &csbi);
                ClearRange(0, csbi.dwSize.Y, fillChar);
            }
        }
    }
}
#endif