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

		LRESULT CALLBACK mouseHookProc(int nCode, WPARAM wParam, LPARAM lParam)
		{
			if (nCode <= 0)
			{
				PMSLLHOOKSTRUCT p = (PMSLLHOOKSTRUCT)lParam;

				//int y = NosStdLib::Global::Console::GetConsoleCursorPosition().Y;
				NosStdLib::Global::Console::ShowCaret(false);
				SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), {0, 0});

				std::wstring output =  std::format(L"{} | {}", p->pt.x, p->pt.y);
				output += std::wstring(NosStdLib::Global::Console::GetConsoleSize().Columns - output.size(), L' ');

				wprintf(output.c_str());
			}

			return CallNextHookEx(MouseHook, nCode, wParam, lParam);
		}

		bool InitializeMouseTracking()
		{
			DWORD prev_mode;
			bool getConsoleStatus = GetConsoleMode(GetStdHandle(STD_INPUT_HANDLE), &prev_mode);
			bool setConsoleStatus = SetConsoleMode(GetStdHandle(STD_INPUT_HANDLE), ENABLE_EXTENDED_FLAGS | (prev_mode & ~ENABLE_QUICK_EDIT_MODE));

			bool MouseHookStatus = (MouseHook = SetWindowsHookEx(WH_MOUSE_LL, NosStdLib::MouseTracking::mouseHookProc, NULL, NULL));

			return ((getConsoleStatus && setConsoleStatus) && MouseHookStatus);
		}
	}
}

#endif