#ifndef _MOUSETRACKING_NOSSTDLIB_HPP_
#define _MOUSETRACKING_NOSSTDLIB_HPP_

#include "../Global.hpp"

#include <Windows.h>
#include <stdio.h>
#include <iostream>

namespace NosStdLib
{
	/// <summary>
	/// namespace that contains mouse tracking functions which are "fun"
	/// </summary>
	namespace MouseTrackingFun
	{
		void ConsoleMSPaint(const PMSLLHOOKSTRUCT& mouseHookStruct, HDC consoleContext)
		{
			if ((GetKeyState(VK_LBUTTON) & 0x8000) != 0)
			{
				int x, y;

				NosStdLib::Global::Console::GetWindowPosition(&x, &y);

				int relX = mouseHookStruct->pt.x - x, relY = (mouseHookStruct->pt.y - y) - 31;

				SetPixel(consoleContext, relX, relY, RGB(255, 255, 255));
			}
		}

		void ConsoleMSPaintInit(HWND* consoleHandle, HDC* consoleContext)
		{
			*consoleHandle = GetConsoleWindow();
			*consoleContext = GetDC(*consoleHandle);
		}
	}

	/// <summary>
	/// Namespace which tracks mouse position
	/// </summary>
	namespace MouseTracking
	{
		HHOOK MouseHook;

		HWND ConsoleHandle;
		HDC ConsoleContext;

		LRESULT CALLBACK mouseHookProc(int nCode, WPARAM wParam, LPARAM lParam)
		{
			if (nCode <= 0)
			{
				PMSLLHOOKSTRUCT mouseHookStruct = (PMSLLHOOKSTRUCT)lParam;

				//int y = NosStdLib::Global::Console::GetConsoleCursorPosition().Y;
				NosStdLib::Global::Console::ShowCaret(false);
				//SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), {0, 0});

				NosStdLib::MouseTrackingFun::ConsoleMSPaint(mouseHookStruct, ConsoleContext);
			}

			return CallNextHookEx(MouseHook, nCode, wParam, lParam);
		}

		/// <summary>
		/// Initialize and create lowlevel Mouse callback
		/// </summary>
		bool InitializeMouseTracking()
		{
			NosStdLib::MouseTrackingFun::ConsoleMSPaintInit(&ConsoleHandle, &ConsoleContext);

			DWORD prev_mode;
			bool getConsoleStatus = GetConsoleMode(GetStdHandle(STD_INPUT_HANDLE), &prev_mode);
			bool setConsoleStatus = SetConsoleMode(GetStdHandle(STD_INPUT_HANDLE), ENABLE_EXTENDED_FLAGS | (prev_mode & ~ENABLE_QUICK_EDIT_MODE));

			bool MouseHookStatus = (MouseHook = SetWindowsHookEx(WH_MOUSE_LL, NosStdLib::MouseTracking::mouseHookProc, NULL, NULL));

			return ((getConsoleStatus && setConsoleStatus) && MouseHookStatus);
		}
	}
}

#endif