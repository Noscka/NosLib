#ifndef _GLOBAL_HPP_
#define _GLOBAL_HPP_

#include <Windows.h>
#include <string>
#include <stringapiset.h>
#include <io.h>
#include <fcntl.h>

namespace NosStdLib
{
	/// <summary>
	/// namespace meant for items which will get regularly used
	/// </summary>
	namespace Global
	{
		/// <summary>
		/// namespace for items which are related to strings (both string and wstring)
		/// </summary>
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

		#pragma region IsNumber
			/// <summary>
			/// Check if wstring is number (with or without signs)
			/// </summary>
			/// <param name="wstr">- wstring to check</param>
			/// <param name="allowSigns">(default = false) - if it should allow signs at the start of the string (-123 or +123)</param>
			/// <returns>if wstring is valid number</returns>
			bool IsNumber(const std::wstring& wstr, bool allowSigns = false)
			{
				/* Iterator int, allows for changing start position */
				int Iteration = 0;
				if (allowSigns)
				{
					/* if allowSigns is true, check if first character is either - or + or a number */
					if ((wstr[0] != L'-' && wstr[0] != L'+') || std::isdigit(wstr[0]))
						return false;

					Iteration = 1; /* Make Iterator go up 1 so for loop doesn't check first character */
				}

				/* Simple for loop, checking if any char isn't a digit */
				for (; Iteration < wstr.size(); Iteration++)
				{
					if (!std::isdigit(wstr[Iteration])) return false;
				}

				return true;
			}

			/// <summary>
			/// Check if string is number (with or without signs)
			/// </summary>
			/// <param name="str">- string to check</param>
			/// <param name="allowSigns">(default = false) - if it should allow signs at the start of the string (-123 or +123)</param>
			/// <returns>if string is valid number</returns>
			bool IsNumber(const std::string& str, bool allowSigns = false)
			{
				/* convert to Wstring and use the Wstring version, less likely to have any error involving data loss. */
				return IsNumber(ToWstring(str), allowSigns);
			}
		#pragma endregion
		}

		/// <summary>
		/// namespace for items which are related to Console
		/// </summary>
		namespace Console
		{
			/// <summary>
			/// namespace for items (mostly functions) which are meant to (but don't have to) be used at the beginning of the program to enable stuff like Unicode characters or ANSI escape code (colored text)
			/// </summary>
			namespace InitializeModifiers
			{
				/// <summary>
				/// Sets program to output unicode16 text
				/// </summary>
				/// <returns>output of function used. -1 if error and anything else if succeful</returns>
				int EnableUnicode()
				{
					return _setmode(_fileno(stdout), _O_U16TEXT); /* set program to unicode output */
				}

			#pragma region EnableANSI
				/// <summary>
				/// Appends `ENABLE_VIRTUAL_TERMINAL_PROCESSING` and `DISABLE_NEWLINE_AUTO_RETURN` to console modes to allow for colored text with Custom Console Handle
				/// </summary>
				/// <param name="consoleHandle">- Custom Console Handle</param>
				/// <returns>if operation was succesful</returns>
				bool EnableANSI(HANDLE consoleHandle)
				{
					DWORD consoleMode;
					bool getOperationResults = GetConsoleMode(consoleHandle, &consoleMode);
					consoleMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING; /* makes output get parsed, so ANSI gets actually used */
					consoleMode |= DISABLE_NEWLINE_AUTO_RETURN; /* makes it so newlines have an effect. */
					bool setOperationResults = SetConsoleMode(consoleHandle, consoleMode);

					return (getOperationResults && setOperationResults); /* return the and of both (only returns true if both are true) */
				}


				/// <summary>
				/// Appends `ENABLE_VIRTUAL_TERMINAL_PROCESSING` and `DISABLE_NEWLINE_AUTO_RETURN` to console modes to allow for colored text
				/// </summary>
				/// <returns>if operation was succesful</returns>
				bool EnableANSI()
				{
					return EnableANSI(GetStdHandle(STD_OUTPUT_HANDLE));
				}
			#pragma endregion
			}

		#pragma region GetConsoleCaretPosition
			/// <summary>
			/// Get position of console caret with Custom Console Handle
			/// </summary>
			/// <param name="consoleHandle">- Custom Console Handle</param>
			/// <returns>COORD position of console caret</returns>
			COORD GetConsoleCaretPosition(HANDLE consoleHandle)
			{
				CONSOLE_SCREEN_BUFFER_INFO csbi;
				if (GetConsoleScreenBufferInfo(consoleHandle, &csbi))
				{
					return csbi.dwCursorPosition;
				}
				else
				{
					// The function failed. Call GetLastError() for details.
					return { 0, 0 };
				}
			}

			/// <summary>
			/// Get position of console caret
			/// </summary>
			/// <returns>COORD position of console caret</returns>
			COORD GetConsoleCaretPosition()
			{
				return GetConsoleCaretPosition(GetStdHandle(STD_OUTPUT_HANDLE));
			}
		#pragma endregion

		#pragma region ClearRange
			/// <summary>
			/// Clear range in console from position with custom Console Handle
			/// </summary>
			/// <param name="consoleHandle">- Custom Console Handle</param>
			/// <param name="position">- position from which to start clearing</param>
			/// <param name="range">- range to which clear</param>
			/// <param name="fillChar">(default = L' ') - the character to clear with</param>
			void ClearRange(HANDLE consoleHandle, int position, int range, wchar_t fillChar = L' ')
			{
				CONSOLE_SCREEN_BUFFER_INFO csbi;
				COORD tl = { 0, (SHORT)(position) };
				GetConsoleScreenBufferInfo(consoleHandle, &csbi);
				DWORD written, cells = csbi.dwSize.X * (1 + range);
				FillConsoleOutputCharacter(consoleHandle, fillChar, cells, tl, &written);
				FillConsoleOutputAttribute(consoleHandle, csbi.wAttributes, cells, tl, &written);
				SetConsoleCursorPosition(consoleHandle, tl);
			}

			/// <summary>
			/// Clear range in console from position
			/// </summary>
			/// <param name="position">- position from which to start clearing</param>
			/// <param name="range">- range to which clear</param>
			/// <param name="fillChar">(default = L' ') - the character to clear with</param>
			void ClearRange(int position, int range, wchar_t fillChar = L' ')
			{
				ClearRange(GetStdHandle(STD_OUTPUT_HANDLE), position, range, fillChar);
			}
		#pragma endregion

		#pragma region ClearLine
			/// <summary>
			/// Clear a single line on position with custom Console Handle
			/// </summary>
			/// <param name="consoleHandle">- Custom Console Handle</param>
			/// <param name="position">- position to clear</param>
			/// <param name="fillChar">(default = L' ') - the character to clear with</param>
			void ClearLine(HANDLE consoleHandle, int position, wchar_t fillChar = L' ')
			{
				ClearRange(consoleHandle, position, 0, fillChar);
			}

			/// <summary>
			/// Clear a single line on position
			/// </summary>
			/// <param name="position">- position to clear</param>
			/// <param name="fillChar">(default = L' ') - the character to clear with</param>
			void ClearLine(int position, wchar_t fillChar = L' ')
			{
				ClearLine(GetStdHandle(STD_OUTPUT_HANDLE), position, fillChar);
			}
		#pragma endregion

		#pragma region ClearScreen
			/// <summary>
			/// Clear whole console with custom Console Handle
			/// </summary>
			/// <param name="consoleHandle">- Custom Console Handle</param>
			/// <param name="fillChar">(default = L' ') - character to clear with</param>
			void ClearScreen(HANDLE consoleHandle, wchar_t fillChar = L' ')
			{
				CONSOLE_SCREEN_BUFFER_INFO csbi;
				GetConsoleScreenBufferInfo(consoleHandle, &csbi);
				ClearRange(consoleHandle, 0, csbi.dwSize.Y, fillChar);
			}

			/// <summary>
			/// clear the whole console
			/// </summary>
			/// <param name="fillChar">(default = L' ') - character to clear with</param>
			void ClearScreen(wchar_t fillChar = L' ')
			{
				ClearScreen(GetStdHandle(STD_OUTPUT_HANDLE), fillChar);
			}
		#pragma endregion

		#pragma region ShowCaret
			/// <summary>
			/// Show or hide Console Caret with custom Console Handle
			/// </summary>
			/// <param name="consoleHandle">- Custom Console Handle</param>
			/// <param name="showFlag">- if caret will be shown or hidden</param>
			void ShowCaret(HANDLE consoleHandle, bool showFlag)
			{
				CONSOLE_CURSOR_INFO cursorInfo;

				GetConsoleCursorInfo(consoleHandle, &cursorInfo);
				cursorInfo.bVisible = showFlag; // set the cursor visibility
				SetConsoleCursorInfo(consoleHandle, &cursorInfo);
			}

			/// <summary>
			/// Show or hide Console Caret
			/// </summary>
			/// <param name="showFlag">- if caret will be shown or hidden</param>
			void ShowCaret(bool showFlag)
			{
				ShowCaret(GetStdHandle(STD_OUTPUT_HANDLE), showFlag);
			}
		#pragma endregion
		}
	}
}
#endif