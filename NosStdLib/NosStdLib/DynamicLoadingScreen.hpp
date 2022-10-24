#ifndef _DYNAMICLOADINGSCREEN_HPP_
#define _DYNAMICLOADINGSCREEN_HPP_

#include "FileManagement.hpp"
#include <Windows.h>
#include <string>

#include "Resource/resource.h"

namespace NosStdLib
{
	class LoadingScreen
	{
	public:
		enum LoadType
		{
			Unknown = 0,
			Known = 1,
		};
	private:
		static inline NosStdLib::FileManagement::FilePath FontFilePath; /* Path to the font life resource */

		std::wstring SplashScreen; /* Splash Screen */
		int SplashScreenYSize, PreviousWriteCoords; /* Needs renaming, intergers */
		float PercentageDone;
		std::wstring StatusMessage;

		static inline CONSOLE_SCREEN_BUFFER_INFO csbi;
		static inline HANDLE ConsoleHandle;
		static inline int columns, rows;

		LoadType BarType;

		template <typename Func, typename ... VariadicArgs>
		void KnownProgressLoad(Func&& callable, VariadicArgs&& ... args)
		{
			wprintf(SplashScreen.c_str());

			std::thread FunctionThread([this](Func&& callable, VariadicArgs&& ... args) { this->ThreadingFunction(callable, std::forward<VariadicArgs>(args)...); }, callable, std::forward<VariadicArgs>(args)...);

			GetConsoleScreenBufferInfo(ConsoleHandle, &csbi);
			columns = csbi.srWindow.Right - csbi.srWindow.Left + 1;

			std::wstring bar = L"";
			int Lenght = 50;

			SetConsoleCursorPosition(ConsoleHandle, { 0, (SHORT)SplashScreenYSize });
			while (PercentageDone < 1 && !CrossThreadFinishBoolean)
			{

				float left = PercentageDone * Lenght;

				bar += std::wstring(floor(left), L'█');

				left -= floor(left);
				bar += std::wstring(floor(left / 0.5), L'▌');

				wprintf((std::wstring(((columns / 2) - Lenght / 2), ' ') + bar + L'\n').c_str());
				wprintf(StatusMessage.c_str());

				Sleep(100);
				rows = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;
				PreviousWriteCoords = SplashScreenYSize;
				SplashScreenYSize = (rows - 4) - (std::count(StatusMessage.begin(), StatusMessage.end(), L'\n'));
				LoadingScreen::ClearRangeLines(PreviousWriteCoords, std::count(StatusMessage.begin(), StatusMessage.end(), L'\n') + 1);
				LoadingScreen::ClearRangeLines(SplashScreenYSize, std::count(StatusMessage.begin(), StatusMessage.end(), L'\n') + 1);
				bar = L"";
			}

			FunctionThread.join();
		}

		template <typename Func, typename ... VariadicArgs>
		void UnknownProgressLoad(Func&& callable, VariadicArgs&& ... args)
		{
			wprintf(SplashScreen.c_str());

			std::thread FunctionThread([this](Func&& callable, VariadicArgs&& ... args) { this->ThreadingFunction(callable, std::forward<VariadicArgs>(args)...); }, callable, std::forward<VariadicArgs>(args)...);
			GetConsoleScreenBufferInfo(ConsoleHandle, &csbi);
			columns = csbi.srWindow.Right - csbi.srWindow.Left + 1;

			std::wstring bar = L"▁ ▂ ▃ ▄ ▅ ▆ ▇ █ ▇ ▆ ▅ ▄ ▃ ▂ ▁";

			int MidPosition = std::ceil((float)bar.length() / 2); /* For tracking the middle character character */
			int TrueMid = std::ceil((float)bar.length() / 2); /* Middle absolute position */
			bool GoingRight = true; /* Tracking the direction in which the bar is going in */

			SetConsoleCursorPosition(ConsoleHandle, { 0, (SHORT)SplashScreenYSize });

			while (PercentageDone < 1 && !CrossThreadFinishBoolean)
			{
				if (MidPosition == 1 || MidPosition == bar.length())
					GoingRight = !GoingRight;
				if (GoingRight)
				{
					wprintf((std::wstring(((columns / 2) - bar.length() / 2), ' ') + MoveRight(&bar) + L'\n').c_str());
					wprintf(StatusMessage.c_str());
					MidPosition++;
				}
				else
				{
					wprintf((std::wstring(((columns / 2) - bar.length() / 2), ' ') + MoveLeft(&bar) + L'\n').c_str());
					wprintf(StatusMessage.c_str());
					MidPosition--;
				}

				int sleepTime = 0;
				if (MidPosition >= TrueMid)
					sleepTime = ((float)(MidPosition + 1) / 15) * 50;
				else
				{
					int Difference = TrueMid - MidPosition;
					sleepTime = ((float)(TrueMid + Difference + 1) / 15) * 50;
				}
				Sleep(sleepTime);
				rows = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;
				PreviousWriteCoords = SplashScreenYSize;
				SplashScreenYSize = (rows - 4) - (std::count(StatusMessage.begin(), StatusMessage.end(), L'\n'));
				LoadingScreen::ClearRangeLines(PreviousWriteCoords, std::count(StatusMessage.begin(), StatusMessage.end(), L'\n') + 1);
				LoadingScreen::ClearRangeLines(SplashScreenYSize, std::count(StatusMessage.begin(), StatusMessage.end(), L'\n') + 1);
			}

			FunctionThread.join();
		}

		template <typename Func, typename ... VariadicArgs>
		void ThreadingFunction(Func&& callable, VariadicArgs&& ... args)
		{
			(*callable)(this, std::forward<VariadicArgs>(args)...);
			(CrossThreadFinishBoolean) = !(CrossThreadFinishBoolean);
		}

		static void ClearRangeLines(int Position, int range)
		{
			CONSOLE_SCREEN_BUFFER_INFO csbi;
			HANDLE ConsoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);

			COORD tl = { 0, (SHORT)(Position) };
			GetConsoleScreenBufferInfo(ConsoleHandle, &csbi);
			DWORD written, cells = csbi.dwSize.X * (1 + range);
			FillConsoleOutputCharacter(ConsoleHandle, ' ', cells, tl, &written);
			FillConsoleOutputAttribute(ConsoleHandle, csbi.wAttributes, cells, tl, &written);
			SetConsoleCursorPosition(ConsoleHandle, tl);
		}
	public:
		static void InitilizeFont(std::wstring Path = LR"(\Resources\)")
		{
			if (!(Path[Path.length() - 1] == L'\\'))
			{
				throw std::invalid_argument("Isn't a valid path. Cannot have file name included in the path");
				return;
			}

			FontFilePath = NosStdLib::FileManagement::FilePath(Path, L"CustomConsola.ttf");

			std::filesystem::create_directories(std::filesystem::path(FontFilePath.GetAbsolutePath())); /* Make Resources Direcory */

			if (!std::filesystem::exists(FontFilePath.GetFilePath())) /* Check if Font file already exists*/
			{
				HINSTANCE hResInstance = (HINSTANCE)GetModuleHandle(NULL);
				HRSRC ResourceHandle = FindResource(hResInstance, MAKEINTRESOURCE(IDR_CUSTOM_CONSOLA_FONT), L"XFONT");

				HGLOBAL ResourceMemory = LoadResource(hResInstance, ResourceHandle);

				void* ResourceData = LockResource(ResourceMemory);

				size_t ResourceLenght = SizeofResource(hResInstance, ResourceHandle);
				HANDLE hFile = CreateFile(FontFilePath.GetFilePath().c_str(), GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, 0, NULL);
				DWORD bytesWritten = ResourceLenght;

				WriteFile(hFile, ResourceData, ResourceLenght, &bytesWritten, NULL);
				CloseHandle(hFile);
			}
			/* Extract font from resource */

			/* Add Font Resource */
			if (AddFontResourceEx(FontFilePath.GetFilePath().c_str(), NULL, NULL) == 0)
			{
				throw std::exception("Font add fails");
			}
			/* Add Font Resource */

			/* Make console use font */
			CONSOLE_FONT_INFOEX cfi = { 0 };

			cfi.cbSize = sizeof(cfi);
			cfi.nFont = 0;
			cfi.dwFontSize.X = 0;
			cfi.dwFontSize.Y = 16;
			cfi.FontFamily = FF_DONTCARE;
			cfi.FontWeight = FW_NORMAL;
			wcscpy_s(cfi.FaceName, L"Custom Consolas");
			SetCurrentConsoleFontEx(GetStdHandle(STD_OUTPUT_HANDLE), TRUE, &cfi);
			/* Make console use font */
		}

		static void TerminateFont()
		{
			/* Remove Font Resource */
			if (RemoveFontResourceEx(FontFilePath.GetFilePath().c_str(), NULL, NULL) == 0)
			{
				throw std::exception("Font remove fails");
			}
			/* Remove Font Resource */
		}

		static std::wstring CenterString(std::wstring input, bool All)
		{
			GetConsoleScreenBufferInfo(ConsoleHandle, &csbi);
			columns = csbi.srWindow.Right - csbi.srWindow.Left + 1;
			if (All)
			{
				std::vector<std::wstring> inputSplit;
				std::wstring output = L"";
				//boost::split(inputSplit, input, boost::is_any_of(L"\n"));

				for (std::wstring Singleinput : inputSplit)
				{
					output += (std::wstring(((columns / 2) - Singleinput.length() / 2), ' ') + Singleinput + L'\n');
				}

				return output;
			}
			else
			{
				return (std::wstring(((columns / 2) - input.length() / 2), ' ') + input + L'\n');
			}
		}

		bool CrossThreadFinishBoolean;

		LoadingScreen(LoadType barType, std::wstring splashScreen = L"")
		{
			BarType = barType;
			SplashScreen = splashScreen;

			PercentageDone = 0;
			CrossThreadFinishBoolean = false;
			ConsoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
		}

		template <typename Func, typename ... VariadicArgs>
		void StartLoading(Func&& callable, VariadicArgs&& ... args)
		{
			GetConsoleScreenBufferInfo(ConsoleHandle, &csbi);
			columns = csbi.srWindow.Right - csbi.srWindow.Left + 1;
			rows = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;

			if (SplashScreen == L"")
				SplashScreenYSize = 0;
			else
			{
				SplashScreenYSize = rows - 4;
			}

			switch (BarType)
			{
			case Unknown:
				UnknownProgressLoad(std::forward<Func>(callable), std::forward<VariadicArgs>(args)...);
				break;
			case Known:
				KnownProgressLoad(std::forward<Func>(callable), std::forward<VariadicArgs>(args)...);
				break;
			}
		}

		void UpdateKnownProgressBar(float percentageDone, std::wstring statusMessage = L"") //, bool centerString = true, bool centerAll = true)
		{
			PercentageDone = percentageDone;
			StatusMessage = statusMessage;

			//if (centerString)
			//	StatusMessage = std::async(std::launch::async, CenterString, statusMessage, centerAll).get();
			//else
			//	StatusMessage = statusMessage;
		}

		void Finish()
		{
			CrossThreadFinishBoolean = true;
		}

		std::wstring MoveRight(std::wstring* string)
		{
			wchar_t LastChar = (*string)[string->length() - 1];
			for (int ii = string->length() - 1; ii >= 0; ii--)
			{
				if (ii == 0)
					(*string)[0] = LastChar;
				else
					(*string)[ii] = (*string)[ii - 1];
			}

			return *string;
		}

		std::wstring MoveLeft(std::wstring* string)
		{
			wchar_t LastChar = (*string)[0];
			for (int ii = 0; ii <= string->length() - 1; ii++)
			{
				if (ii == string->length() - 1)
				{
					(*string)[ii] = LastChar;
				}
				else
					(*string)[ii] = (*string)[ii + 1];
			}

			return *string;
		}
	};
}

#endif