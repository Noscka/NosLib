#ifndef _CONSOLE_NosLib_HPP_
#define _CONSOLE_NosLib_HPP_

#include "String.hpp"
#include "DimensionVector.hpp"

#include <Windows.h>
#include <io.h>
#include <fcntl.h>

namespace NosLib
{
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
			inline int EnableUnicode()
			{
				return _setmode(_fileno(stdout), _O_U16TEXT); /* set program to unicode output */
			}

		#pragma region EnableANSI
			/// <summary>
			/// Appends `ENABLE_VIRTUAL_TERMINAL_PROCESSING` and `DISABLE_NEWLINE_AUTO_RETURN` to console modes to allow for colored text with Custom Console Handle
			/// </summary>
			/// <param name="consoleHandle">- Custom Console Handle</param>
			/// <returns>if operation was succesful</returns>
			inline bool EnableANSI(const HANDLE& consoleHandle)
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
			inline bool EnableANSI()
			{
				return EnableANSI(GetStdHandle(STD_OUTPUT_HANDLE));
			}
		#pragma endregion

		#pragma region BeautifyConsole
			/// <summary>
			/// Makes the console loop more beatiful with a specified console window
			/// </summary>
			/// <typeparam name="CharT">- string char type</typeparam>
			/// <param name="window">- specified window handle</param>
			/// <param name="title">- what the window should be called</param>
			/// <returns>if succesful</returns>
			template <typename CharT>
			inline bool BeatifyConsole(const HWND& window, const std::basic_string<CharT>& title)
			{
				bool setTitle;
			#ifdef UNICODE
				setTitle = SetConsoleTitleW(NosLib::String::ConvertString<wchar_t, CharT>(title).c_str());
			#else
				setTitle = SetConsoleTitleA(NosLib::String::ConvertString<char, CharT>(title).c_str());
			#endif // !UNICODE

				return setTitle && ShowScrollBar(window, SB_BOTH, FALSE);
			}

			/// <summary>
			/// Makes the console loop more beatiful
			/// </summary>
			/// <typeparam name="CharT">- string char type</typeparam>
			/// <param name="title">- what the window should be called</param>
			/// <returns>if succesful</returns>
			template <typename CharT>
			inline bool BeatifyConsole(const std::basic_string<CharT>& title)
			{
				return BeatifyConsole<CharT>(GetConsoleWindow(), title);
			}
		#pragma endregion

#ifdef _EVENTHANDLING_NOSDOS_HPP_
			/// <summary>
			/// (Aliased) Creates hook which handles events
			/// </summary>
			/// <returns>true for succesful and false for unsuccesful</returns>
			inline bool (*InitializeEventHandler)() = &NosLib::EventHandling::InitializeEventHandler;
#endif
		}

	#pragma region GetCaretPosition
		/// <summary>
		/// Get position of console caret with Custom Console Handle
		/// </summary>
		/// <param name="consoleHandle">- Custom Console Handle</param>
		/// <returns>COORD position of console caret</returns>
		inline COORD GetCaretPosition(const HANDLE& consoleHandle)
		{
			CONSOLE_SCREEN_BUFFER_INFO csbi;
			if (GetConsoleScreenBufferInfo(consoleHandle, &csbi))
			{
				return csbi.dwCursorPosition;
			}
			else
			{
				// The function failed. Call GetLastError() for details.
				return {0, 0};
			}
		}

		/// <summary>
		/// Get position of console caret
		/// </summary>
		/// <returns>COORD position of console caret</returns>
		inline COORD GetCaretPosition()
		{
			return GetCaretPosition(GetStdHandle(STD_OUTPUT_HANDLE));
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
		inline void ClearRange(const HANDLE& consoleHandle, const int& position, const int& range, const wchar_t& fillChar = L' ')
		{
			CONSOLE_SCREEN_BUFFER_INFO csbi;
			COORD tl = {0, (SHORT)(position)};
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
		inline void ClearRange(const int& position, const int& range, const wchar_t& fillChar = L' ')
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
		inline void ClearLine(const HANDLE& consoleHandle, const int& position, const wchar_t& fillChar = L' ')
		{
			ClearRange(consoleHandle, position, 0, fillChar);
		}

		/// <summary>
		/// Clear a single line on position
		/// </summary>
		/// <param name="position">- position to clear</param>
		/// <param name="fillChar">(default = L' ') - the character to clear with</param>
		inline void ClearLine(const int& position, const wchar_t& fillChar = L' ')
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
		inline void ClearScreen(const HANDLE& consoleHandle, const wchar_t& fillChar = L' ')
		{
			CONSOLE_SCREEN_BUFFER_INFO csbi;
			GetConsoleScreenBufferInfo(consoleHandle, &csbi);
			ClearRange(consoleHandle, 0, csbi.dwSize.Y, fillChar);
		}

		/// <summary>
		/// clear the whole console
		/// </summary>
		/// <param name="fillChar">(default = L' ') - character to clear with</param>
		inline void ClearScreen(const wchar_t& fillChar = L' ')
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
		inline void ShowCaret(const HANDLE& consoleHandle, const bool& showFlag)
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
		inline void ShowCaret(const bool& showFlag)
		{
			ShowCaret(GetStdHandle(STD_OUTPUT_HANDLE), showFlag);
		}
	#pragma endregion

	#pragma region GetWindowPosition
		/// <summary>
		/// Get Window Position with custom console handle
		/// </summary>
		/// <param name="hWnd">- console handle</param>
		/// <param name="x">- pointer to x int</param>
		/// <param name="y">- pointer to y int</param>
		/// <returns>if succesful or not</returns>
		inline bool GetWindowPosition(const HWND& hWnd, int* x, int* y)
		{
			RECT rect = {NULL};
			if (GetWindowRect(hWnd, &rect))
			{
				*x = rect.left;
				*y = rect.top;
				return true;
			}
			return false;
		}

		/// <summary>
		/// Get Window Position
		/// </summary>
		/// <param name="x">- pointer to x int</param>
		/// <param name="y">- pointer to y int</param>
		/// <returns>if succesful or not</returns>
		inline bool GetWindowPosition(int* x, int* y)
		{
			return GetWindowPosition(GetConsoleWindow(), x, y);
		}
	#pragma endregion

	#pragma region GetWindowDimensions
		/// <summary>
		/// Get Window Dimensions with custom console handle
		/// </summary>
		/// <param name="hWnd">- console handle</param>
		/// <returns>Dimensions, will return -1 for 1x, 1y, 2x, 2y if went wrong</returns>
		inline NosLib::Dimension::DimensionD2<LONG> GetWindowDimensions(const HWND& hWnd)
		{
			NosLib::Dimension::DimensionD2<LONG> output(-1,-1,-1,-1);

			RECT rect = {NULL};
			if (GetWindowRect(hWnd, &rect))
			{
				output.UpdateDimension(rect.left, rect.top, rect.right, rect.bottom);
			}

			return output;
		}

		/// <summary>
		/// Get Window Dimensions
		/// </summary>
		/// <returns>Dimensions, will return -1 for 1x, 1y, 2x, 2y if went wrong</returns>
		inline NosLib::Dimension::DimensionD2<LONG> GetWindowDimensions()
		{
			return GetWindowDimensions(GetConsoleWindow());
		}
	#pragma endregion

		/// <summary>
		/// a struct to represent ConsoleSize with Columns and Rows members
		/// </summary>
		struct ConsoleSizeStruct
		{
			int Columns;
			int Rows;
		};

	#pragma region GetConsoleSize
		/// <summary>
		/// Gets console size with no parameters (uses default console handle and CONSOLE_SCREEN_BUFFER_INFO)
		/// </summary>
		/// <returns>ConsoleSizeStruct</returns>
		inline ConsoleSizeStruct GetConsoleSize()
		{
			CONSOLE_SCREEN_BUFFER_INFO ConsoleScreenBI;
			GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &ConsoleScreenBI);
			return {ConsoleScreenBI.srWindow.Right - ConsoleScreenBI.srWindow.Left + 1, ConsoleScreenBI.srWindow.Bottom - ConsoleScreenBI.srWindow.Top + 1};
		}

		/// <summary>
		/// Gets console size with custom CONSOLE_SCREEN_BUFFER_INFO. doesn't update it
		/// </summary>
		/// <param name="consoleScreenBI">- the Console_screen_buffer_info to use</param>
		/// <returns>ConsoleSizeStruct</returns>
		inline ConsoleSizeStruct GetConsoleSize(const CONSOLE_SCREEN_BUFFER_INFO& consoleScreenBI)
		{
			return {consoleScreenBI.srWindow.Right - consoleScreenBI.srWindow.Left + 1, consoleScreenBI.srWindow.Bottom - consoleScreenBI.srWindow.Top + 1};
		}

		/// <summary>
		/// Gets console size with custom ConsoleHandle
		/// </summary>
		/// <param name="ConsoleHandle">- the Console Handle to use</param>
		/// <returns>ConsoleSizeStruct</returns>
		inline ConsoleSizeStruct GetConsoleSize(const HANDLE& ConsoleHandle)
		{
			CONSOLE_SCREEN_BUFFER_INFO ConsoleScreenBI;
			GetConsoleScreenBufferInfo(ConsoleHandle, &ConsoleScreenBI);
			return {ConsoleScreenBI.srWindow.Right - ConsoleScreenBI.srWindow.Left + 1, ConsoleScreenBI.srWindow.Bottom - ConsoleScreenBI.srWindow.Top + 1};
		}

		/// <summary>
		/// Gets console size with custom ConsoleHandle and CONSOLE_SCREEN_BUFFER_INFO. updates CONSOLE_SCREEN_BUFFER_INFO with consoleHandle
		/// </summary>
		/// <param name="ConsoleHandle">- the Console Handle to use</param>
		/// <param name="consoleScreenBI">- a pointer to Console_screen_buffer_info to use</param>
		/// <returns>ConsoleSizeStruct</returns>
		inline ConsoleSizeStruct GetConsoleSize(const HANDLE& ConsoleHandle, CONSOLE_SCREEN_BUFFER_INFO* consoleScreenBI)
		{
			GetConsoleScreenBufferInfo(ConsoleHandle, consoleScreenBI);
			return {consoleScreenBI->srWindow.Right - consoleScreenBI->srWindow.Left + 1, consoleScreenBI->srWindow.Bottom - consoleScreenBI->srWindow.Top + 1};
		}
	#pragma endregion

#pragma region SetConsoleBuffer
		/// <summary>
		/// Set a console buffer size with a custom handle and size
		/// </summary>
		/// <param name="consoleHandle">- custom buffer</param>
		/// <param name="newBufferSize">- custom size</param>
		/// <returns>if succesful</returns>
		inline bool SetConsoleBuffer(const HANDLE& consoleHandle, const COORD& newBufferSize)
		{
			return SetConsoleScreenBufferSize(consoleHandle, newBufferSize);
		}

		/// <summary>
		/// Set a console buffer size to only the visiable amount with a custom handle
		/// </summary>
		/// <param name="consoleHandle">- custom handle</param>
		/// <returns>if succesful</returns>
		inline bool SetConsoleBuffer(const HANDLE& consoleHandle)
		{
			// retrieve screen buffer info
			CONSOLE_SCREEN_BUFFER_INFO scrBufferInfo;
			GetConsoleScreenBufferInfo(consoleHandle, &scrBufferInfo);

			return SetConsoleBuffer(consoleHandle, { (SHORT)scrBufferInfo.dwSize.X , (SHORT)(scrBufferInfo.srWindow.Bottom - scrBufferInfo.srWindow.Top + 1) });
		}

		/// <summary>
		/// Set a console buffer size to only the visiable amount
		/// </summary>
		/// <returns>if succesful</returns>
		inline bool SetConsoleBuffer()
		{
			return SetConsoleBuffer(GetStdHandle(STD_OUTPUT_HANDLE));
		}

		/// <summary>
		/// Set a console buffer size with a custom size
		/// </summary>
		/// <param name="consoleHandle">- custom buffer</param>
		/// <returns>if succesful</returns>
		inline bool SetConsoleBuffer(const COORD& newBufferSize)
		{
			return SetConsoleBuffer(GetStdHandle(STD_OUTPUT_HANDLE), newBufferSize);
		}
#pragma endregion

	#pragma region NoneBlockingMessageLoop
		/// <summary>
		/// function which will create a none blocking message loop. used more as a template
		/// </summary>
		/// <param name="timeoutMilliseconds">(default = 5) - amount of milliseconds to wait for message</param>
		inline void NoneBlockingMessageLoop(DWORD timeoutMilliseconds = 5)
		{
			MSG msg;

			while (true)
			{
				switch (MsgWaitForMultipleObjects(0, NULL, FALSE, timeoutMilliseconds, QS_ALLINPUT))
				{
				case WAIT_OBJECT_0:
					PeekMessage(&msg, NULL, 0, 0, PM_REMOVE);
					TranslateMessage(&msg);
					DispatchMessage(&msg);
					break;

				case WAIT_TIMEOUT:
					wprintf(L"timed out\n");
					break;
				}
			}
		}
	#pragma endregion
	}
}

#endif