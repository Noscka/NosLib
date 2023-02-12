#ifndef _MOUSETRACKING_NOSSTDLIB_HPP_
#define _MOUSETRACKING_NOSSTDLIB_HPP_

#include "../Console.hpp"
#include "../RGB.hpp"
#include "../String.hpp"
#include "../Cast.hpp"
#include "Button.hpp"

#include <Windows.h>
#include <stdio.h>
#include <iostream>
#include <future>

namespace NosStdLib
{
	/// <summary>
	/// namespace that contains mouse tracking functions which are "fun"
	/// </summary>
	namespace MouseTrackingFun
	{
		namespace NosPaint
		{
			HWND ConsoleHandle;
			HDC ConsoleContext;

			NosStdLib::RGB::NosRGB paintColor(255, 0, 0, 1);

			enum BrushType : uint8_t
			{
				square = 0,
				star = 1,
			};

			/// <summary>
			/// Function which actually makes the "MSPaint" work. gets put inside the mouse hook callback function.
			/// </summary>
			/// <param name="mouseHookStruct">- mouse hook struct (gotten from casting lParam)</param>
			/// <param name="brushThickness">- how thick the brush is</param>
			/// <param name="brushType">(default = BrushType::square) - the brush type</param>
			void ConsoleMSPaint(const PMSLLHOOKSTRUCT& mouseHookStruct, const int& brushThickness, const BrushType& brushType = BrushType::square)
			{
				if ((GetKeyState(VK_LBUTTON) & 0x8000) != 0) /* check if left mouse button is pressed */
				{
					int windowX, windowY;
					NosStdLib::Console::GetWindowPosition(&windowX, &windowY); /* get the coords of the window */

					/* position inside the display (black text area) */
					int consoleDisplayX = (mouseHookStruct->pt.x - windowX) - 6,  /* x coord */
						consoleDisplayY = (mouseHookStruct->pt.y - windowY) - 31; /* y coord */

					paintColor.Iterate();

					switch (brushType)
					{
					case BrushType::square:
						for (int i = (-1 * brushThickness) / 2; i <= brushThickness / 2; i++)
						{
							for (int j = (-1 * brushThickness) / 2; j <= brushThickness / 2; j++)
							{
								SetPixel(ConsoleContext, consoleDisplayX + i, consoleDisplayY + j, paintColor);
							}
						}
						break;

					case BrushType::star:
						for (int i = (-1 * brushThickness) / 2; i <= brushThickness / 2; i++)
						{
							SetPixel(ConsoleContext, consoleDisplayX + i, consoleDisplayY, paintColor);
						}
						for (int i = (-1 * brushThickness) / 2; i <= brushThickness / 2; i++)
						{
							SetPixel(ConsoleContext, consoleDisplayX, consoleDisplayY + i, paintColor);
						}
						break;
					}
				}
			}

			/// <summary>
			/// Function needed to make ConsoleMSPaint actually run
			/// </summary>
			void ConsoleMSPaintInit()
			{
				wprintf(L"Press any button to enter NosPaint"); _getch();

				ConsoleContext = GetDC(GetConsoleWindow());
			}
		}

		namespace CharMouseTracking
		{
			COORD LastPosition;

			void CharCoordPrint(const COORD& currentPosition)
			{
				NosStdLib::Console::ConsoleSizeStruct size = NosStdLib::Console::GetConsoleSize();

				if (LastPosition.X == currentPosition.X && LastPosition.Y == currentPosition.Y)
				{
					return;
				}
				else if ((currentPosition.X < 0 || currentPosition.Y < 0) || (currentPosition.X > size.Columns || currentPosition.Y > size.Rows))
				{
					SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), LastPosition);
					wprintf(L" ");
					return;
				}

				SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), LastPosition);
				wprintf(L" ");
				SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), currentPosition);
				wprintf(L"█");

				LastPosition = currentPosition;
			}

			/// <summary>
			/// Calculates the position of the mouse in character terms
			/// </summary>
			/// <param name="mouseHookStruct">- for finding mouse position</param>
			void CalcCharPixel(const PMSLLHOOKSTRUCT& mouseHookStruct)
			{
				int windowX, windowY;
				NosStdLib::Console::GetWindowPosition(&windowX, &windowY); /* get the coords of the window */

				/* position inside the display (black text area) */
				int consoleDisplayX = (mouseHookStruct->pt.x - windowX) - 6,  /* x coord */
					consoleDisplayY = (mouseHookStruct->pt.y - windowY) - 31; /* y coord */

				CONSOLE_FONT_INFOEX consoleFontInfo;
				consoleFontInfo.cbSize = sizeof(CONSOLE_FONT_INFOEX);

				GetCurrentConsoleFontEx(GetStdHandle(STD_OUTPUT_HANDLE), false, &consoleFontInfo);

				CharCoordPrint({NosStdLib::Cast::Cast<SHORT, int>((consoleDisplayX / consoleFontInfo.dwFontSize.X)), NosStdLib::Cast::Cast<SHORT, int>((consoleDisplayY / consoleFontInfo.dwFontSize.Y))});
				SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), {0, 0});
			}
		}
	}

	/// <summary>
	/// Namespace which tracks mouse position
	/// </summary>
	namespace MouseTracking
	{
		HHOOK MouseHook;

		NosStdLib::Vector::VectorD2<int16_t> lastPosition;

		void ButtonManagement(const PMSLLHOOKSTRUCT& mouseHookStruct, const WPARAM& wParam)
		{
			int windowX, windowY;
			NosStdLib::Console::GetWindowPosition(&windowX, &windowY); /* get the coords of the window */

			CONSOLE_FONT_INFOEX consoleFontInfo;
			consoleFontInfo.cbSize = sizeof(CONSOLE_FONT_INFOEX);

			GetCurrentConsoleFontEx(GetStdHandle(STD_OUTPUT_HANDLE), false, &consoleFontInfo);

			int16_t charX = (((mouseHookStruct->pt.x - windowX) - 6) / consoleFontInfo.dwFontSize.X),
					charY = (((mouseHookStruct->pt.y - windowY) - 31) / consoleFontInfo.dwFontSize.Y);

			NosStdLib::Vector::VectorD2<int16_t> currentPosition(NosStdLib::Cast::Cast<int16_t, int>(charX), NosStdLib::Cast::Cast<int16_t, int>(charY));

			switch (wParam)
			{
			case WM_LBUTTONDOWN:
				for (NosStdLib::Button::Button* buttonPointer : NosStdLib::Button::Button::CheckButtonAtPosition(currentPosition))
				{
					buttonPointer->OnClick->TriggerEvent();
				}
				break;

			case WM_MOUSEMOVE:
				break;
			}

			lastPosition = currentPosition;
		}

		LRESULT CALLBACK MouseHookProc(int nCode, WPARAM wParam, LPARAM lParam)
		{
			if (nCode <= 0)
			{
				PMSLLHOOKSTRUCT mouseHookStruct = (PMSLLHOOKSTRUCT)lParam;

				NosStdLib::Console::ShowCaret(false);

				ButtonManagement(mouseHookStruct, wParam);
			}

			return CallNextHookEx(MouseHook, nCode, wParam, lParam);
		}

		/// <summary>
		/// Initialize and create lowlevel Mouse callback
		/// </summary>
		bool InitializeMouseTracking()
		{
			DWORD prev_mode;
			return ((GetConsoleMode(GetStdHandle(STD_INPUT_HANDLE), &prev_mode) &&
					SetConsoleMode(GetStdHandle(STD_INPUT_HANDLE), ENABLE_EXTENDED_FLAGS | (prev_mode & ~ENABLE_QUICK_EDIT_MODE))) &&
					(MouseHook = SetWindowsHookEx(WH_MOUSE_LL, NosStdLib::MouseTracking::MouseHookProc, NULL, NULL)));
		}
	}
}

#endif