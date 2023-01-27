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

				std::wcout << p->pt.x << L" | " << p->pt.y << std::endl;
			}

			return CallNextHookEx(MouseHook, nCode, wParam, lParam);
		}

		bool InitializeMouseTracking()
		{
			DWORD prev_mode;
			bool getConsoleStatus = GetConsoleMode(GetStdHandle(STD_OUTPUT_HANDLE), &prev_mode);
			bool setConsoleStatus = SetConsoleMode(GetStdHandle(STD_OUTPUT_HANDLE), ENABLE_EXTENDED_FLAGS | (prev_mode & ~ENABLE_QUICK_EDIT_MODE));

			wprintf(NosStdLib::Global::ErrorHandling::GetLastErrorAsString().c_str());

			bool MouseHookStatus = (MouseHook = SetWindowsHookEx(WH_MOUSE_LL, NosStdLib::MouseTracking::mouseHookProc, NULL, NULL));

			wprintf(getConsoleStatus ? L"Succesful\n" : L"Failed\n");
			wprintf(setConsoleStatus ? L"Succesful\n" : L"Failed\n");
			wprintf(MouseHookStatus ? L"Succesful\n" : L"Failed\n");

			return ((getConsoleStatus && setConsoleStatus) && MouseHookStatus);
		}
	}
}

#endif