#ifndef _MOUSETRACKING_NosLib_HPP_
#define _MOUSETRACKING_NosLib_HPP_

#include "../Console.hpp"
#include "../RGB.hpp"
#include "../String.hpp"
#include "../Cast.hpp"
#include "../Clickable.hpp"

#include <Windows.h>
#include <stdio.h>
#include <iostream>
#include <future>
#include <thread>

namespace NosLib
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

			NosLib::RGB::NosRGB paintColor(255, 0, 0, 1);

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
			inline void ConsoleMSPaint(const PMSLLHOOKSTRUCT& mouseHookStruct, const int& brushThickness, const BrushType& brushType = BrushType::square)
			{
				if ((GetKeyState(VK_LBUTTON) & 0x8000) != 0) /* check if left mouse button is pressed */
				{
					int windowX, windowY;
					NosLib::Console::GetWindowPosition(&windowX, &windowY); /* get the coords of the window */

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
			inline void ConsoleMSPaintInit()
			{
				wprintf(L"Press any button to enter NosPaint"); _getch();

				ConsoleContext = GetDC(GetConsoleWindow());
			}
		}

		namespace CharMouseTracking
		{
			COORD LastPosition;

			inline void CharCoordPrint(const COORD& currentPosition)
			{
				NosLib::Console::ConsoleSizeStruct size = NosLib::Console::GetConsoleSize();

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
			inline void CalcCharPixel(const PMSLLHOOKSTRUCT& mouseHookStruct)
			{
				int windowX, windowY;
				NosLib::Console::GetWindowPosition(&windowX, &windowY); /* get the coords of the window */

				/* position inside the display (black text area) */
				int consoleDisplayX = (mouseHookStruct->pt.x - windowX) - 6,  /* x coord */
					consoleDisplayY = (mouseHookStruct->pt.y - windowY) - 31; /* y coord */

				CONSOLE_FONT_INFOEX consoleFontInfo;
				consoleFontInfo.cbSize = sizeof(CONSOLE_FONT_INFOEX);

				GetCurrentConsoleFontEx(GetStdHandle(STD_OUTPUT_HANDLE), false, &consoleFontInfo);

				CharCoordPrint({NosLib::Cast<SHORT, int>((consoleDisplayX / consoleFontInfo.dwFontSize.X)), NosLib::Cast<SHORT, int>((consoleDisplayY / consoleFontInfo.dwFontSize.Y))});
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
		DWORD PreviousConsoleMode;

		NosLib::Vector::VectorD2<int16_t> lastPosition;

		inline void ButtonManagement(const PMSLLHOOKSTRUCT& mouseHookStruct, const WPARAM& wParam)
		{
			NosLib::Dimension::DimensionD2<LONG> windowDimensions = NosLib::Console::GetWindowDimensions(); /* get the dimensions of the window */

			if (!(windowDimensions.CheckIfPositionInside(mouseHookStruct->pt))) /* if outside of window, just ignore */
			{
				return;
			}

			CONSOLE_FONT_INFOEX consoleFontInfo{sizeof(CONSOLE_FONT_INFOEX)};
			GetCurrentConsoleFontEx(GetStdHandle(STD_OUTPUT_HANDLE), false, &consoleFontInfo);

			SCROLLINFO info{sizeof(SCROLLINFO), SIF_POS};
			GetScrollInfo(GetConsoleWindow(), SB_VERT, &info); /* Get scroll bar info to add to the CharY position */

			int16_t charX = NosLib::Cast<int16_t>(((mouseHookStruct->pt.x - windowDimensions.PointOne.X) - 6 /* Left+Right border sizes */) / consoleFontInfo.dwFontSize.X),
				charY = NosLib::Cast<int16_t>((((mouseHookStruct->pt.y - windowDimensions.PointOne.Y) - 31 /* Top border size */) / consoleFontInfo.dwFontSize.Y) + info.nPos);

			NosLib::Vector::VectorD2<int16_t> currentPosition(charX, charY);

			switch (wParam)
			{
			case WM_LBUTTONDOWN:
				NosLib::Clickable::Clickable::TriggerClickEventAtPosition(currentPosition);
				break;

			case WM_MOUSEMOVE:
				NosLib::Clickable::Clickable::TriggerHoverEventAtPosition(currentPosition, lastPosition);
				break;
			case WM_MOUSEWHEEL:
				//short scrollMagnitude = GET_WHEEL_DELTA_WPARAM(mouseHookStruct->mouseData) / WHEEL_DELTA;
				// TODO: IMPLEMENT SCROLL WHEEL CONTROLS
				//std::wcout << L"Scroll Magnitude: " << scrollMagnitude << std::endl;
				break;
			}

			lastPosition = currentPosition;
		}

		LRESULT CALLBACK MouseHookProc(int nCode, WPARAM wParam, LPARAM lParam)
		{
			if (nCode <= HC_ACTION)
			{
				PMSLLHOOKSTRUCT mouseHookStruct = (PMSLLHOOKSTRUCT)lParam;

				ButtonManagement(mouseHookStruct, wParam);
			}

			return CallNextHookEx(MouseHook, nCode, wParam, lParam);
		}

		/// <summary>
		/// Initialize and create low level Mouse callback hook
		/// </summary>
		/// <returns>true if successful, false if not</returns>
		inline bool InitializeMouseTracking()
		{
			return (GetConsoleMode(GetStdHandle(STD_INPUT_HANDLE), &PreviousConsoleMode) &&
					SetConsoleMode(GetStdHandle(STD_INPUT_HANDLE), ENABLE_EXTENDED_FLAGS | (PreviousConsoleMode & ~ENABLE_QUICK_EDIT_MODE))
					&& (MouseHook = SetWindowsHookEx(WH_MOUSE_LL, NosLib::MouseTracking::MouseHookProc, GetModuleHandle(NULL), 0)));
		}

		/// <summary>
		/// Terminates LowLevel mouse callback hook without setting console back to highlightable
		/// </summary>
		/// <returns>true if successful, false if not</returns>
		inline bool TemporaryTerminateMouseTracking()
		{
			return UnhookWindowsHookEx(MouseHook);
		}

		/// <summary>
		/// Terminates LowLevel mouse callback hook
		/// </summary>
		/// <returns>true if successful, false if not</returns>
		inline bool TerminateMouseTracking()
		{
			return (UnhookWindowsHookEx(MouseHook) && SetConsoleMode(GetStdHandle(STD_INPUT_HANDLE), PreviousConsoleMode));
		}
	}
}

#endif