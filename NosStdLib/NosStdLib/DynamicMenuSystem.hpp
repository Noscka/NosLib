#ifndef _DYNAMICMENUSYSTEM_NOSSTDLIB_HPP_
#define _DYNAMICMENUSYSTEM_NOSSTDLIB_HPP_

#include "Definitions.hpp"
#include "UnicodeTextGenerator.hpp"
#include "DynamicMenuSystem/MenuEntry.hpp"

#include <Windows.h>

namespace NosStdLib
{
	namespace Menu
	{
		/// <summary>
		/// the main Menu class which will be used to render and display menu
		/// </summary>
		class DynamicMenu
		{
		private:
			std::wstring Title;										/* Menu Title */
			HANDLE ConsoleHandle;									/* global Console Handle so it is synced across all operations and so it doesn't have to retrieved */
			CONSOLE_SCREEN_BUFFER_INFO ConsoleScreenBI;				/* global ConsoleScreenBI so it is synced across all operations */
			NosStdLib::Console::ConsoleSizeStruct ConsoleSizeStruct;/* a struct container for the Console colums and rows */
			NosStdLib::DynamicArray<MenuEntryBase*> MenuEntryList;	/* array of MenuEntries */

			bool MenuLoop,				/* if the menu should continue looping (true -> yes, false -> no) */
				 GenerateUnicodeTitle,	/* if to generate a big Unicode title */
				 AddExitEntry,			/* if to add a quit option/entry at the bottom */
				 CenterTitle,			/* if the title should be centered */
				 AddedQuit;				/* if quit entry was already added. TODO: store int of position and if more entries are added (last isn't quit), move quit to last */
		public:
			DynamicMenu(const std::wstring& title, const bool& generateUnicodeTitle = true, const bool& addExitEntry = true, const bool& centerTitle = true)
			{
				Title = title;
				AddExitEntry = addExitEntry;
				GenerateUnicodeTitle = generateUnicodeTitle;
				CenterTitle = centerTitle;

				ConsoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
			}

			/// <summary>
			/// Starts the menu
			/// </summary>
			void StartMenu()
			{
				MenuLoop = true; /* incase menu was quit before */

				if (AddExitEntry && !AddedQuit)
				{
					AddMenuEntry(new MenuEntry(L"Quit", new NosStdLib::Functional::FunctionStore(&QuitMenu, this)));
					AddedQuit = true;
				}

				int ch, exCh; /* for getting input data */
				int currentIndex = 0; /* Which item is currently selected */
				int oldIndex = currentIndex; /* Old index to know old position */
				int titleSize = 0; /* title size (for calculations where actual menu entries start) */
				int lastMenuSize = MenuEntryList.GetArrayIndexPointer(); /* for checking if the menu has increased/descreased */
				ConsoleSizeStruct = NosStdLib::Console::GetConsoleSize(ConsoleHandle, &ConsoleScreenBI); /* Update the ConsoleSize first time */
				NosStdLib::Console::ConsoleSizeStruct oldConsoleSizeStruct = ConsoleSizeStruct;

				DrawMenu(currentIndex, &titleSize); /* Draw menu first time */

				NosStdLib::Console::ShowCaret(false); /* Hide the caret */

				while (MenuLoop)
				{
					ch = _getch(); /* first character input */
					if (ch == NosStdLib::Definitions::ENTER)
					{ /* WARNING: Might need to show the caret again not mattering what EntryType it is, as for some functions. it might be necessary */
						EntryInputPassStruct InputPassStruct{ currentIndex, titleSize, EntryInputPassStruct::InputType::Enter, false };
						MenuEntryList[currentIndex]->EntryInput(&InputPassStruct);
						if (InputPassStruct.Redraw)
							DrawMenu(currentIndex, &titleSize);
						NosStdLib::Console::ShowCaret(false); /* hide the caret again */
					}
					else if (!(ch && ch != 224))
					{
						switch (exCh = _getch())
						{
						case NosStdLib::Definitions::ARROW_UP:
							if (currentIndex > 0) /* Decrement only if larger the 0 */
							{
								currentIndex--; /* Decrement the Indenetation */
							}
							break;
						case NosStdLib::Definitions::ARROW_DOWN:
							if (currentIndex < MenuEntryList.GetArrayIndexPointer() - 1) /* Increment only if smaller then List size */
							{
								currentIndex++; /* Increment the Indenetation */
							}
							break;
						case NosStdLib::Definitions::ARROW_LEFT:
							{
								EntryInputPassStruct InputPassStruct{ currentIndex, titleSize, EntryInputPassStruct::InputType::ArrowLeft, false };
								MenuEntryList[currentIndex]->EntryInput(&InputPassStruct);
								if (InputPassStruct.Redraw)
									DrawMenu(currentIndex, &titleSize);
								break;
							}
						case NosStdLib::Definitions::ARROW_RIGHT:
							{
								EntryInputPassStruct InputPassStruct{ currentIndex, titleSize, EntryInputPassStruct::InputType::ArrowRight, false };
								MenuEntryList[currentIndex]->EntryInput(&InputPassStruct);
								if (InputPassStruct.Redraw)
									DrawMenu(currentIndex, &titleSize);
								break;
							}
						}
					}

					ConsoleSizeStruct = NosStdLib::Console::GetConsoleSize(ConsoleHandle, &ConsoleScreenBI);
					/* if the console dimentions have changed (console window has increased or decreased). then redraw whole menu */
					if (oldConsoleSizeStruct.Columns != ConsoleSizeStruct.Columns || oldConsoleSizeStruct.Rows != ConsoleSizeStruct.Rows)
					{
						oldConsoleSizeStruct = ConsoleSizeStruct;
						NosStdLib::Console::ShowCaret(false); /* hide the caret again */
						DrawMenu(currentIndex, &titleSize);
					}

					/*
						What needs to be redrawing depending on if its up for down
						if the index goes down (bigger number), you need to clear above and current line
						|| Old Selected Entry
						\/ New Selected Entry <-- Here is Cursor
						and if going up (smaller number)
						/\ New Selected Entry <-- Here is Cursor
						|| Old Selected Entry
					*/

					COORD finalPosition = {0,0};

					if (currentIndex > oldIndex && oldIndex != currentIndex) /* Going Down */
					{
						SetConsoleCursorPosition(ConsoleHandle, { 0, (SHORT)(titleSize + currentIndex - 1) });
						wprintf((MenuEntryList[oldIndex]->EntryString(false) + MenuEntryList[currentIndex]->EntryString(true)).c_str());

						if ((titleSize + currentIndex) + (ConsoleSizeStruct.Rows / 2) < 0)
							finalPosition = { 0,0 };
						else if ((titleSize + currentIndex) + (ConsoleSizeStruct.Rows / 2) > MenuEntryList.GetArrayIndexPointer())
							finalPosition = { 0, (SHORT)(MenuEntryList.GetArrayIndexPointer() + titleSize - 1) };
						else
							finalPosition = { 0, (SHORT)((titleSize + currentIndex) + (ConsoleSizeStruct.Rows / 2)) };
					}
					else if(oldIndex != currentIndex)/* Going Up */
					{
						SetConsoleCursorPosition(ConsoleHandle, { 0, (SHORT)(titleSize + currentIndex) });
						wprintf((MenuEntryList[currentIndex]->EntryString(true) + MenuEntryList[oldIndex]->EntryString(false)).c_str());

						if ((titleSize + currentIndex) - (ConsoleSizeStruct.Rows / 2) < 0)
							finalPosition = { 0,0 };
						else if ((titleSize + currentIndex) - (ConsoleSizeStruct.Rows / 2) > MenuEntryList.GetArrayIndexPointer())
							finalPosition = { 0, (SHORT)MenuEntryList.GetArrayIndexPointer() };
						else
							finalPosition = { 0, (SHORT)((titleSize + currentIndex) - (ConsoleSizeStruct.Rows / 2)) };

					}

					SetConsoleCursorPosition(ConsoleHandle, finalPosition);

					oldIndex = currentIndex;
				}
				NosStdLib::Console::ClearScreen(); /* Clear the screen to remove the menu */
				NosStdLib::Console::ShowCaret(true); /* show the caret again */
			}

			/// <summary>
			/// Adds entry to menu
			/// </summary>
			/// <param name="Entry">- the entry to add</param>
			void AddMenuEntry(MenuEntryBase* Entry)
			{
				Entry->SetEntryVariables(&ConsoleHandle, &ConsoleScreenBI, &ConsoleSizeStruct);
				MenuEntryList.Append(Entry);
			}

		private:
			/// <summary>
			/// Draws the menu
			/// </summary>
			/// <param name="CurrentIndex">- currrent index</param>
			/// <param name="TitleSize">- pointer to the title size int so it can be calculated</param>
			void DrawMenu(const int& currentIndex, int* titleSize)
			{
				NosStdLib::Console::ClearScreen();

				ConsoleSizeStruct = NosStdLib::Console::GetConsoleSize(ConsoleHandle, &ConsoleScreenBI);

				std::wstring outputString; /* string for full "display" as it is the most perfomace efficent method */

				if (GenerateUnicodeTitle) /* If custom Title is true, its going to use the straight characters instead of generating a unicode title */
					outputString = NosStdLib::UnicodeTextGenerator::BasicUnicodeTextGenerate(ConsoleHandle, Title, CenterTitle); // add title with "ascii generator"
				else
					if (CenterTitle)
						outputString = std::wstring(((ConsoleSizeStruct.Columns / 2) - Title.length() / 2), ' ') + Title;
					else
						outputString = Title;

				*titleSize = std::count(outputString.begin(), outputString.end(), L'\n');

				// for loop using counter to get the index so to add the >< to the selected option
				for (int i = 0; i < MenuEntryList.GetArrayIndexPointer(); i++)
				{
					if (i == currentIndex)
						outputString += MenuEntryList[i]->EntryString(true);
					else
						outputString += MenuEntryList[i]->EntryString(false);
				}

				wprintf(outputString.c_str());

				SetConsoleCursorPosition(ConsoleHandle, { 0, (SHORT)(currentIndex) });
			}

			/// <summary>
			/// quits the menu
			/// </summary>
			/// <param name="menuPointer">- a pointer to self</param>
			static void QuitMenu(DynamicMenu* menuPointer)
			{
				menuPointer->MenuLoop = false;
			}
		};

	#pragma region Template Specialization
	#pragma region DynamicMenu
		/// <summary>
		/// create a wstring which shows the Entry name, value and is also centered
		/// </summary>
		/// <param name="selected">- if the entry is selected or not</param>
		/// <returns>wstring which shows the Entry name, value and is also centered</returns>
		std::wstring MenuEntry<DynamicMenu>::EntryString(const bool& selected)
		{
			*MenuConsoleSizeStruct = NosStdLib::Console::GetConsoleSize(*MenuConsoleHandle, MenuConsoleScreenBI); /* Update values */

			std::wstring output = (selected ?
								   std::wstring(((MenuConsoleSizeStruct->Columns / 2) - EntryName.length() / 2) - 3, ' ') + NosStdLib::RGB::NosRGB(212, 155, 55).MakeANSICode<wchar_t>() + L">> " + EntryName + L" <<" :
								   std::wstring(((MenuConsoleSizeStruct->Columns / 2) - EntryName.length() / 2) - 0, ' ') + NosStdLib::RGB::NosRGB(212, 155, 55).MakeANSICode<wchar_t>() + L"" + EntryName + L"");
			output += std::wstring(max((MenuConsoleSizeStruct->Columns - (output.size() + ((MenuConsoleSizeStruct->Columns / 2) - output.size() / 2))), 0), L' ') + L"\033[0m\n";
			return output;
		}

		/// <summary>
		/// pass/send a input to the Entry
		/// </summary>
		/// <param name="inputType">- input that is getting sent</param>
		void MenuEntry<DynamicMenu>::EntryInput(EntryInputPassStruct* inputStruct)
		{
			switch (inputStruct->inputType)
			{
			case EntryInputPassStruct::InputType::Enter:
				NosStdLib::Console::ClearScreen();
				TypePointerStore->StartMenu();
				inputStruct->Redraw = true;
				break;
			case EntryInputPassStruct::InputType::ArrowLeft:
				break;
			case EntryInputPassStruct::InputType::ArrowRight:
				break;
			}
		}
	#pragma endregion
	#pragma endregion
	}
}
#endif