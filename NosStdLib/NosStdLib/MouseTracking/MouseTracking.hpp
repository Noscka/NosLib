#ifndef _MOUSETRACKING_NOSSTDLIB_HPP_
#define _MOUSETRACKING_NOSSTDLIB_HPP_

#include "../Global.hpp"

#include <Windows.h>
#include <stdio.h>
#include <iostream>

namespace NosStdLib
{
	/// <summary>
	/// Namespace which tracks mouse position
	/// </summary>
	namespace MouseTracking
	{
		HHOOK MouseHook;

		HWND myconsole;
		HDC mydc;

		LRESULT CALLBACK mouseHookProc(int nCode, WPARAM wParam, LPARAM lParam)
		{
			if (nCode <= 0)
			{
				PMSLLHOOKSTRUCT p = (PMSLLHOOKSTRUCT)lParam;

				//int y = NosStdLib::Global::Console::GetConsoleCursorPosition().Y;
				NosStdLib::Global::Console::ShowCaret(false);
				//SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), {0, 0});

				if ((GetKeyState(VK_LBUTTON) & 0x8000) != 0)
				{
					int x, y;

					NosStdLib::Global::Console::GetWindowPosition(myconsole, &x, &y);

					int relX = p->pt.x - x, relY = (p->pt.y - y) - 30;

					SetPixel(mydc, relX, relY, RGB(255, 255, 255));
				}
			}

			return CallNextHookEx(MouseHook, nCode, wParam, lParam);
		}

		bool InitializeMouseTracking()
		{
			myconsole = GetConsoleWindow();
			mydc = GetDC(myconsole);

			DWORD prev_mode;
			bool getConsoleStatus = GetConsoleMode(GetStdHandle(STD_INPUT_HANDLE), &prev_mode);
			bool setConsoleStatus = SetConsoleMode(GetStdHandle(STD_INPUT_HANDLE), ENABLE_EXTENDED_FLAGS | (prev_mode & ~ENABLE_QUICK_EDIT_MODE));

			bool MouseHookStatus = (MouseHook = SetWindowsHookEx(WH_MOUSE_LL, NosStdLib::MouseTracking::mouseHookProc, NULL, NULL));

			return ((getConsoleStatus && setConsoleStatus) && MouseHookStatus);
		}
	}
}

#endif