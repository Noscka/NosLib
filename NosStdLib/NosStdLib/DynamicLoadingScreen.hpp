#ifndef _DYNAMICLOADINGSCREEN_HPP_
#define _DYNAMICLOADINGSCREEN_HPP_

#include "Global.hpp"
#include "FileManagement.hpp"
#include <Windows.h>
#include <string>
#include <math.h>

#include "Resource/resource.h"

namespace NosStdLib
{
	class LoadingScreen
	{
	public:
		enum LoadType
		{
			Unknown = 0,	/* progress cannot be counted or it is unknown */
			Known = 1,		/* progress can be counted */
		};
	private:
		static inline NosStdLib::FileManagement::FilePath FontFilePath; /* Path to the font life resource */

		std::wstring SplashScreen;					/* Splash Screen */
		int CurrentWriteRow, PreviousWriteRow;		/* Current and previous loading bar write row  */
		float PercentageDone;						/* decimal percentage of progress */
		std::wstring StatusMessage;					/* Status message (Might need to move to local instead of global) */
		bool CenterStatusMesage;					/* if the StatusMessage should get centered. takes stress off the work thread and gets done on the drawing thread */

		static inline HANDLE ConsoleHandle;				/* global and static Console Handle */
		static inline CONSOLE_SCREEN_BUFFER_INFO csbi;	/* global and static Console ScreenBI */
		static inline int columns, rows;				/* global and static colums and rows */

		LoadType BarType; /* bar type of the object */

		/// <summary>
		/// Update all needed variables and clear text
		/// </summary>
		void MidOperationUpdate()
		{
			GetConsoleScreenBufferInfo(ConsoleHandle, &csbi); /* update console info */
			rows = csbi.srWindow.Bottom - csbi.srWindow.Top + 1; /* recalculate rows */
			columns = csbi.srWindow.Right - csbi.srWindow.Left + 1; /* and columns */
			PreviousWriteRow = CurrentWriteRow; /* before recalculating new writing row, save it incase its different and the old one needs clearing */

			/* recalculate writing row, either 4 above the bottom (with status message) or right below the splash screen */
			CurrentWriteRow = max((rows - 4) - (std::count(StatusMessage.begin(), StatusMessage.end(), L'\n')), (std::count(SplashScreen.begin(), SplashScreen.end(), L'\n') + 1));

			if (CurrentWriteRow != PreviousWriteRow) /* if CurrentWriteRow and PreviousWriteRow are not equal (write position changed), clear previous */
				NosStdLib::Global::Console::ClearRange(PreviousWriteRow, std::count(StatusMessage.begin(), StatusMessage.end(), L'\n') + 1);
		}

		/// <summary>
		/// function which displays bar which knows the progress
		/// </summary>
		/// <typeparam name="Func">- callable type</typeparam>
		/// <typeparam name="...VariadicArgs">- the parameter's of the callable</typeparam>
		/// <param name="callable">- the callable</param>
		/// <param name="...args">- the parameters of the callable</param>
		template <typename Func, typename ... VariadicArgs>
		void KnownProgressLoad(Func&& callable, VariadicArgs&& ... args)
		{
			wprintf(SplashScreen.c_str());

			std::thread FunctionThread([this](Func&& callable, VariadicArgs&& ... args) { this->ThreadingFunction(callable, std::forward<VariadicArgs>(args)...); }, callable, std::forward<VariadicArgs>(args)...);

			GetConsoleScreenBufferInfo(ConsoleHandle, &csbi);
			columns = csbi.srWindow.Right - csbi.srWindow.Left + 1;

			std::wstring bar = L"";

			SetConsoleCursorPosition(ConsoleHandle, { 0, (SHORT)CurrentWriteRow });
			while (PercentageDone < 1 && !CrossThreadFinishBoolean)
			{
				int maxLenght = max(columns - 60, 20);
				float left = PercentageDone * maxLenght;

				bar += std::wstring((left / 1.0), L'█');
				
				bar += std::wstring(fmod(left, 1.0)/ 0.5, L'▌');

				SetConsoleCursorPosition(ConsoleHandle, { 0, (SHORT)CurrentWriteRow });
				wprintf((std::wstring(max(((columns / 2) - maxLenght / 2), 0), L' ') + bar + std::wstring(max((columns - (bar.size() + ((columns / 2) - maxLenght / 2))), 0), L' ') + L"\n").c_str());
				wprintf(CenterStatusMesage ? NosStdLib::Global::String::CenterString(StatusMessage, true).c_str() : StatusMessage.c_str());

				Sleep(100);
				MidOperationUpdate();
				bar = L"";
			}

			FunctionThread.join();
		}

		/// <summary>
		/// function which displays bar in which the progress is unknown
		/// </summary>
		/// <typeparam name="Func">- callable type</typeparam>
		/// <typeparam name="...VariadicArgs">- the parameter's of the callable</typeparam>
		/// <param name="callable">- the callable</param>
		/// <param name="...args">- the parameters of the callable</param>
		template <typename Func, typename ... VariadicArgs>
		void UnknownProgressLoad(Func&& callable, VariadicArgs&& ... args)
		{
			wprintf(SplashScreen.c_str());

			std::thread FunctionThread([this](Func&& callable, VariadicArgs&& ... args) { this->ThreadingFunction(callable, std::forward<VariadicArgs>(args)...); }, callable, std::forward<VariadicArgs>(args)...);
			GetConsoleScreenBufferInfo(ConsoleHandle, &csbi);
			columns = csbi.srWindow.Right - csbi.srWindow.Left + 1;

			std::wstring bar = L"▁ ▂ ▃ ▄ ▅ ▆ ▇ █ ▇ ▆ ▅ ▄ ▃ ▂ ▁";

			int maxLenght = bar.size(); /* for efficiency so string.size() doesn't have to get recalculated each time */
			int MidPosition = std::ceil((float)bar.length() / 2); /* For tracking the middle character character */
			int TrueMid = std::ceil((float)bar.length() / 2); /* Middle absolute position */
			bool GoingRight = true; /* Tracking the direction in which the bar is going in */

			SetConsoleCursorPosition(ConsoleHandle, { 0, (SHORT)CurrentWriteRow });
			while (PercentageDone < 1 || !CrossThreadFinishBoolean)
			{
				if (MidPosition == 1 || MidPosition == bar.length())
					GoingRight = !GoingRight;
				if (GoingRight)
				{
					SetConsoleCursorPosition(ConsoleHandle, { 0, (SHORT)CurrentWriteRow });
					wprintf((std::wstring(max(((columns / 2) - maxLenght / 2), 0), L' ') + MoveRight(&bar) + std::wstring(max((columns - (bar.size() + ((columns / 2) - maxLenght / 2))), 0), L' ') + L"\n").c_str());
					wprintf(CenterStatusMesage ? NosStdLib::Global::String::CenterString(StatusMessage, true).c_str() : StatusMessage.c_str());
					MidPosition++;
				}
				else
				{
					SetConsoleCursorPosition(ConsoleHandle, { 0, (SHORT)CurrentWriteRow });
					wprintf((std::wstring(max(((columns / 2) - maxLenght / 2), 0), L' ') + MoveLeft(&bar) + std::wstring(max((columns - (bar.size() + ((columns / 2) - maxLenght / 2))), 0), L' ') + L"\n").c_str());
					wprintf(CenterStatusMesage ? NosStdLib::Global::String::CenterString(StatusMessage, true).c_str() : StatusMessage.c_str());
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
				MidOperationUpdate();
			}

			FunctionThread.join();
		}

		/// <summary>
		/// function which calls the callable and onces its finished, makes finish bool false
		/// </summary>
		/// <typeparam name="Func">- callable type</typeparam>
		/// <typeparam name="...VariadicArgs">- the parameter's of the callable</typeparam>
		/// <param name="callable">- the callable</param>
		/// <param name="...args">- the parameters of the callable</param>
		template <typename Func, typename ... VariadicArgs>
		void ThreadingFunction(Func&& callable, VariadicArgs&& ... args)
		{
			(*callable)(this, std::forward<VariadicArgs>(args)...);
			(CrossThreadFinishBoolean) = true;
		}
	public:
		/// <summary>
		/// Initilizes font resource by getting it from exe, putting it in a place and then adding it to memory
		/// </summary>
		/// <param name="Path">- relative path where to put font (change or allow for absolute)</param>
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

		/// <summary>
		/// Remove font as resource (free it)
		/// </summary>
		static void TerminateFont()
		{
			/* Remove Font Resource */
			if (RemoveFontResourceEx(FontFilePath.GetFilePath().c_str(), NULL, NULL) == 0)
			{
				throw std::exception("Font remove fails");
			}
			/* Remove Font Resource */
		}

		bool CrossThreadFinishBoolean; /* bool which determens if loading screen thread should still load or not (false means continue loading, true means stop loading) */

		/// <summary>
		/// create LoadingScreen object
		/// </summary>
		/// <param name="barType">- the bar type which will be displayed when started</param>
		/// <param name="splashScreen">(default = L"") - what the should display above the bar</param>
		/// <param name="centerString">(default = true) - if the splashScreen should be centered</param>
		LoadingScreen(LoadType barType, std::wstring splashScreen = L"", bool centerString = true)
		{
			BarType = barType;
			SplashScreen = centerString ? NosStdLib::Global::String::CenterString(splashScreen, true) : splashScreen;

			PercentageDone = 0;
			CrossThreadFinishBoolean = false;
			ConsoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
		}

		/// <summary>
		/// the function that starts loading. 
		/// </summary>
		/// <typeparam name="Func">- callable type</typeparam>
		/// <typeparam name="...VariadicArgs">- the parameter's of the callable</typeparam>
		/// <param name="callable">- the callable</param>
		/// <param name="...args">- the parameters of the callable</param>
		template <typename Func, typename ... VariadicArgs>
		void StartLoading(Func&& callable, VariadicArgs&& ... args)
		{
			GetConsoleScreenBufferInfo(ConsoleHandle, &csbi);
			columns = csbi.srWindow.Right - csbi.srWindow.Left + 1;
			rows = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;

			CrossThreadFinishBoolean = false;

			CurrentWriteRow = rows - 4;

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

		/// <summary>
		/// Function used inside the function that is being loaded, updates different parameters
		/// </summary>
		/// <param name="percentageDone">- Percent done in decimal form</param>
		/// <param name="statusMessage">- status message that gets displayed below the loading bar</param>
		void UpdateKnownProgressBar(float percentageDone, std::wstring statusMessage = L"", bool centerStatusMessage = true) //, bool centerString = true, bool centerAll = true)
		{
			PercentageDone = percentageDone;
			StatusMessage = statusMessage;
			CenterStatusMesage = centerStatusMessage;
			//if (centerString)
			//	StatusMessage = std::async(std::launch::async, CenterString, statusMessage, centerAll).get();
			//else
			//	StatusMessage = statusMessage;
		}

		/// <summary>
		/// Finish loading
		/// </summary>
		void Finish()
		{
			CrossThreadFinishBoolean = true;
		}

		// TODO: put MoveRight and MoveLeft to Global namespace

		/// <summary>
		/// Move the contents of a string right by 1 space
		/// </summary>
		/// <param name="string">- the string to move right</param>
		/// <returns>modified string</returns>
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
		
		/// <summary>
		/// Move the contents of a string left by 1 space
		/// </summary>
		/// <param name="string">- the string to move left</param>
		/// <returns>modified string</returns>
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