#ifndef _EXPERIMENTAL_NOSLIB_HPP_
#define _EXPERIMENTAL_NOSLIB_HPP_
#ifdef _WIN32

#include "Console.hpp"
#include "RGB.hpp"

#include <Windows.h>
#include <string>

#include <conio.h>

namespace NosLib
{
	/// <summary>
	/// namespace which contains functions and classes which get used for testing (this namespace will most likely not have comments)
	/// </summary>
	namespace TestEnv
	{
		/*[coroutine] TODO: research, test and implement coroutines
		int CoroutineTest()
		{

		}*/

		namespace PointerRoots
		{
			template<typename T>
			inline void RootRelease(T& v)
			{
				if constexpr (std::is_pointer_v<std::remove_pointer_t<T>>)
				{
					RootRelease<std::remove_pointer_t<T>>(*v);
				}
				else
				{
					delete v;
				}
			}

			class destructionTesting
			{
			private:
				int FirstIndex;
				int SecondIndex;
				std::wstring MemoryHolder;
			public:
				destructionTesting()
				{
					wprintf(L"Created Default\n");
				}

				destructionTesting(int firstIndex, int secondIndex)
				{
					FirstIndex = firstIndex;
					SecondIndex = secondIndex;
					MemoryHolder = std::wstring(1000, L'A');
					wprintf(std::format(L"Created: {},{}\n", FirstIndex, SecondIndex).c_str());
				}

				~destructionTesting()
				{
					wprintf(std::format(L"destroyed: {},{}\n", FirstIndex, SecondIndex).c_str());
				}
			};

			inline void Running()
			{
				wprintf(L"Press any button to start"); _getch();

				int amount = 100;

				destructionTesting** ptr = new destructionTesting * [amount]();

				for (int i = 0; i <= amount; i++)
				{
					ptr[i] = new destructionTesting(0, i);
				}

				wprintf(L"Press any button to destroy"); _getch();

				delete[] ptr;

				//for (int i = 0; i <= amount; i++)
				//{
				//    delete ptr[i];
				//}

				RootRelease<destructionTesting**>(ptr);
			}
		}

		namespace Fun
		{
			inline LPPOINT GetCaretPositionReturn()
			{
				LPPOINT point = new POINT();
				GetCaretPos(point);
				return point;
			}

			inline std::wstring DrawSquare(int position, int columnCount)
			{
				std::wstring ANSIEscapeCodeStart = NosLib::RGB::NosRGB(20, 180, 170).MakeANSICode<wchar_t>();
				std::wstring LeftPadding = std::wstring(max(position - 1, 0), L' ');
				std::wstring BoxCharacter = L"|";
				std::wstring RightPadding = std::wstring(max(columnCount - position, 0), L' ');
				std::wstring ANSIEscapeCodeEnd = L"\033[0m";

				return  ANSIEscapeCodeStart + LeftPadding + BoxCharacter + RightPadding + ANSIEscapeCodeEnd;
			}

			inline void IterateSquare(int sleepSpeed = 15)
			{
				NosLib::Console::ShowCaret(false);

				HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
				CONSOLE_SCREEN_BUFFER_INFO csbi;
				GetConsoleScreenBufferInfo(consoleHandle, &csbi);
				int columns = csbi.srWindow.Right - csbi.srWindow.Left + 1;
				int rows = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;

				for (int i = 0; i <= columns; i++)
				{
					wprintf((NosLib::TestEnv::Fun::DrawSquare(i, columns) + L'\n').c_str());
					wprintf((NosLib::TestEnv::Fun::DrawSquare(i, columns) + L'\n').c_str());
					wprintf((NosLib::TestEnv::Fun::DrawSquare(i, columns) + L'\n').c_str());
					wprintf((NosLib::TestEnv::Fun::DrawSquare(i, columns) + L'\n').c_str());
					SetConsoleCursorPosition(consoleHandle, { 0, 0 });
					Sleep(sleepSpeed);
				}

				for (int i = columns; i > 0; i--)
				{
					wprintf((NosLib::TestEnv::Fun::DrawSquare(i, columns) + L'\n').c_str());
					wprintf((NosLib::TestEnv::Fun::DrawSquare(i, columns) + L'\n').c_str());
					wprintf((NosLib::TestEnv::Fun::DrawSquare(i, columns) + L'\n').c_str());
					wprintf((NosLib::TestEnv::Fun::DrawSquare(i, columns) + L'\n').c_str());
					SetConsoleCursorPosition(consoleHandle, { 0, 0 });
					Sleep(sleepSpeed);
				}

				NosLib::Console::ShowCaret(true);
			}
		}
	}
}

#endif
#endif