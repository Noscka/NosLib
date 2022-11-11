#ifndef _DYNAMICMENUSYSTEM_NOSSTDLIB_HPP_
#define _DYNAMICMENUSYSTEM_NOSSTDLIB_HPP_

#include "Global.hpp"
#include "DynamicArray.hpp"
#include "UnicodeTextGenerator.hpp"
#include "Functional.hpp"
#include "TextColor.hpp"

#include <Windows.h>
#include <conio.h>
#include <string>
#include <functional>
#include <type_traits>

namespace NosStdLib
{
	namespace Menu
	{
		constexpr int ARROW_UP = 72;
		constexpr int ARROW_DOWN = 80;
		constexpr int ARROW_LEFT = 75;
		constexpr int ARROW_RIGHT = 77;

		constexpr int ENTER = 13;
		constexpr int BACKSPACE = 8;

		class DynamicMenu;

		struct EntryInputPassStruct
		{
			enum InputType : uint8_t
			{
				Enter,
				ArrowLeft,
				ArrowRight,
			};

			int CurrentIndex;
			int TitleSize;
			InputType inputType;
			bool Redraw;
		};

		/// <summary>
		/// The base of MenuEntry which only exists to allow for storing all the templated types together
		/// </summary>
		class MenuEntryBase
		{
		protected:
			std::wstring EntryName; /* The entry name */

			HANDLE* MenuConsoleHandle;												/* a pointer to the menu's MenuConsoleHandle, so its synced */
			CONSOLE_SCREEN_BUFFER_INFO* MenuConsoleScreenBI;						/* a pointer to the menu's MenuConsoleScreenBI, so its synced */
			NosStdLib::Global::Console::ConsoleSizeStruct* MenuConsoleSizeStruct;	/* a pointer to the menu's MenuConsoleSizeStruct, so its synced */
		public:
			/// <summary>
			/// create a wstring which shows the Entry name, value and is also centered
			/// </summary>
			/// <param name="selected">- if the entry is selected or not</param>
			/// <returns>wstring which shows the Entry name, value and is also centered</returns>
			virtual std::wstring EntryString(bool selected)
			{
				return EntryName;
			}

			/// <summary>
			/// pass/send a input to the Entry
			/// </summary>
			/// <param name="inputType">- input that is getting sent</param>
			virtual void EntryInput(EntryInputPassStruct* inputStruct)
			{
				return;
			}

			/// <summary>
			/// Update/set the variables such as pointers to ConsoleHandle, ConsoleScreenBI and ConsoleSizeStruct
			/// </summary>
			/// <returns></returns>
			void SetEntryVariables(HANDLE* menuConsoleHandle, CONSOLE_SCREEN_BUFFER_INFO* menuConsoleScreenBI, NosStdLib::Global::Console::ConsoleSizeStruct* menuConsoleSizeStruct)
			{
				MenuConsoleHandle = menuConsoleHandle;
				MenuConsoleScreenBI = menuConsoleScreenBI;
				MenuConsoleSizeStruct = menuConsoleSizeStruct;
			}
		};

		/// <summary>
		/// Class which is used for entries in the DynamicMenu class, contains the necessary data
		/// </summary>
		/// <typeparam name="EntryType">- the type of entry</typeparam>
		template<class EntryType>
		class MenuEntry : public MenuEntryBase
		{
		private:
			// Type specific vars
			EntryType* TypePointerStore;
		public:

			//// <summary>
			/// For Arrays
			/// </summary>
			MenuEntry() {}
			
			/// <summary>
			/// Function Entry
			/// </summary>
			/// <param name="name">- entry name</param>
			/// <param name="typePointerStore">- pointer to variable of type</param>
			MenuEntry(std::wstring name, EntryType* typePointerStore)
			{
				EntryName = name;
				TypePointerStore = typePointerStore;
			}

			/// <summary>
			/// create a wstring which shows the Entry name, value and is also centered
			/// </summary>
			/// <param name="selected">- if the entry is selected or not</param>
			/// <returns>wstring which shows the Entry name, value and is also centered</returns>
			std::wstring EntryString(bool selected)
			{
				*MenuConsoleSizeStruct = NosStdLib::Global::Console::GetConsoleSize(*MenuConsoleHandle, MenuConsoleScreenBI); /* Update values */

				int SpaceLenght = ((MenuConsoleSizeStruct->Columns / 2) - EntryName.length() / 2);

				if constexpr (std::is_base_of_v<NosStdLib::Functional::FunctionStoreBase, EntryType>)
				{
					std::wstring output = (selected ? 
										   std::wstring(SpaceLenght - 3, ' ') + L">> " + EntryName + L" <<" :
										   std::wstring(SpaceLenght - 0, ' ') + L"" + EntryName + L"");
					output += std::wstring(max((MenuConsoleSizeStruct->Columns - (output.size() + ((MenuConsoleSizeStruct->Columns / 2) - output.size() / 2))), 0), L' ') + L'\n';
					return output;
				}
				else
				{
					return L"generic";
				}
			}

			/// <summary>
			/// pass/send a input to the Entry
			/// </summary>
			/// <param name="inputType">- input that is getting sent</param>
			void EntryInput(EntryInputPassStruct* inputStruct)
			{
				if constexpr (std::is_base_of_v<NosStdLib::Functional::FunctionStoreBase, EntryType>)
				{
					switch (inputStruct->inputType)
					{
					case EntryInputPassStruct::InputType::Enter:
						NosStdLib::Global::Console::ClearScreen();
						TypePointerStore->RunFunction();
						inputStruct->Redraw = true;
						break;
					case EntryInputPassStruct::InputType::ArrowLeft:
						break;
					case EntryInputPassStruct::InputType::ArrowRight:
						break;
					}
				}
				else
				{
					return;
				}
			}
		};

		/// <summary>
		/// the main Menu class which will be used to render and display menu
		/// </summary>
		class DynamicMenu
		{
		private:
			std::wstring Title;												/* Menu Title */
			HANDLE ConsoleHandle;											/* global Console Handle so it is synced across all operations and so it doesn't have to retrieved */
			CONSOLE_SCREEN_BUFFER_INFO ConsoleScreenBI;						/* global ConsoleScreenBI so it is synced across all operations */
			NosStdLib::Global::Console::ConsoleSizeStruct ConsoleSizeStruct;/* a struct container for the Console colums and rows */
			NosStdLib::DynamicArray<MenuEntryBase*> MenuEntryList;			/* array of MenuEntries */

			bool MenuLoop,				/* if the menu should continue looping (true -> yes, false -> no) */
				 GenerateUnicodeTitle,	/* if to generate a big Unicode title */
				 AddExitEntry,			/* if to add a quit option/entry at the bottom */
				 CenterTitle,			/* if the title should be centered */
				 AddedQuit;				/* if quit entry was already added. TODO: store int of position and if more entries are added (last isn't quit), move quit to last */
		public:
			DynamicMenu(std::wstring title, bool generateUnicodeTitle = true, bool addExitEntry = true, bool centerTitle = true)
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
				ConsoleSizeStruct = NosStdLib::Global::Console::GetConsoleSize(ConsoleHandle, &ConsoleScreenBI); /* Update the ConsoleSize first time */
				NosStdLib::Global::Console::ConsoleSizeStruct oldConsoleSizeStruct = ConsoleSizeStruct;

				DrawMenu(currentIndex, &titleSize); /* Draw menu first time */

				NosStdLib::Global::Console::ShowCaret(false); /* Hide the caret */

				while (MenuLoop)
				{
					ch = _getch(); /* first character input */
					if (ch == ENTER)
					{ /* WARNING: Might need to show the caret again not mattering what EntryType it is, as for some functions. it might be necessary */
						EntryInputPassStruct InputPassStruct{ currentIndex, titleSize, EntryInputPassStruct::InputType::Enter, false };
						MenuEntryList[currentIndex]->EntryInput(&InputPassStruct);
						if (InputPassStruct.Redraw)
							DrawMenu(currentIndex, &titleSize);
						NosStdLib::Global::Console::ShowCaret(false); /* hide the caret again */
					}
					else if (!(ch && ch != 224))
					{
						switch (exCh = _getch())
						{
						case ARROW_UP:
							if (currentIndex > 0) /* Decrement only if larger the 0 */
							{
								currentIndex--; /* Decrement the Indenetation */
							}
							break;
						case ARROW_DOWN:
							if (currentIndex < MenuEntryList.GetArrayIndexPointer() - 1) /* Increment only if smaller then List size */
							{
								currentIndex++; /* Increment the Indenetation */
							}
							break;
						case ARROW_LEFT:
							{
								EntryInputPassStruct InputPassStruct{ currentIndex, titleSize, EntryInputPassStruct::InputType::ArrowLeft, false };
								MenuEntryList[currentIndex]->EntryInput(&InputPassStruct);
								if (InputPassStruct.Redraw)
									DrawMenu(currentIndex, &titleSize);
								break;
							}
						case ARROW_RIGHT:
							{
								EntryInputPassStruct InputPassStruct{ currentIndex, titleSize, EntryInputPassStruct::InputType::ArrowRight, false };
								MenuEntryList[currentIndex]->EntryInput(&InputPassStruct);
								if (InputPassStruct.Redraw)
									DrawMenu(currentIndex, &titleSize);
								break;
							}
						}
					}

					ConsoleSizeStruct = NosStdLib::Global::Console::GetConsoleSize(ConsoleHandle, &ConsoleScreenBI);
					/* if the console dimentions have changed (console window has increased or decreased). then redraw whole menu */
					if (oldConsoleSizeStruct.Columns != ConsoleSizeStruct.Columns || oldConsoleSizeStruct.Rows != ConsoleSizeStruct.Rows)
					{
						oldConsoleSizeStruct = ConsoleSizeStruct;
						NosStdLib::Global::Console::ShowCaret(false); /* hide the caret again */
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
				NosStdLib::Global::Console::ClearScreen(); /* Clear the screen to remove the menu */
				NosStdLib::Global::Console::ShowCaret(true); /* show the caret again */
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
				NosStdLib::Global::Console::ClearScreen();

				ConsoleSizeStruct = NosStdLib::Global::Console::GetConsoleSize(ConsoleHandle, &ConsoleScreenBI);

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

	#pragma region FunctionSpecialization
	#pragma region bool
		/// <summary>
		/// create a wstring which shows the Entry name, value and is also centered
		/// </summary>
		/// <param name="selected">- if the entry is selected or not</param>
		/// <returns>wstring which shows the Entry name, value and is also centered</returns>
		std::wstring MenuEntry<bool>::EntryString(bool selected)
		{
			*MenuConsoleSizeStruct = NosStdLib::Global::Console::GetConsoleSize(*MenuConsoleHandle, MenuConsoleScreenBI); /* Update values */

			std::wstring output = (selected ? 
								   std::wstring(((MenuConsoleSizeStruct->Columns / 2) - EntryName.length() / 2) - 3, ' ') + L">> " + EntryName + std::wstring(4, ' ') + (*TypePointerStore ? L"[X]" : L"[ ]") +L" <<" :
								   std::wstring(((MenuConsoleSizeStruct->Columns / 2) - EntryName.length() / 2) - 0, ' ') + L"" + EntryName + std::wstring(4, ' ') + (*TypePointerStore ? L"[X]" : L"[ ]") + L"");
			output += std::wstring(max((MenuConsoleSizeStruct->Columns - (output.size() + ((MenuConsoleSizeStruct->Columns / 2) - output.size() / 2))), 0), L' ') + L'\n';
			return output;
		}

		/// <summary>
		/// pass/send a input to the Entry
		/// </summary>
		/// <param name="inputType">- input that is getting sent</param>
		void MenuEntry<bool>::EntryInput(EntryInputPassStruct* inputStruct)
		{
			switch (inputStruct->inputType)
			{
			case EntryInputPassStruct::InputType::Enter:
				(*TypePointerStore) = !(*TypePointerStore);
				SetConsoleCursorPosition(*MenuConsoleHandle, { 0, (SHORT)(inputStruct->CurrentIndex + inputStruct->TitleSize) });
				wprintf(EntryString(true).c_str());
				break;
			case EntryInputPassStruct::InputType::ArrowLeft:
				break;
			case EntryInputPassStruct::InputType::ArrowRight:
				break;
			}
		}
	#pragma endregion

	#pragma region int
		/// <summary>
		/// create a wstring which shows the Entry name, value and is also centered
		/// </summary>
		/// <param name="selected">- if the entry is selected or not</param>
		/// <returns>wstring which shows the Entry name, value and is also centered</returns>
		std::wstring MenuEntry<int>::EntryString(bool selected)
		{
			*MenuConsoleSizeStruct = NosStdLib::Global::Console::GetConsoleSize(*MenuConsoleHandle, MenuConsoleScreenBI); /* Update values */

			std::wstring output = (selected ? 
								   std::wstring(((MenuConsoleSizeStruct->Columns / 2) - EntryName.length() / 2), ' ') + EntryName + std::wstring(4, ' ') + L"<" + std::to_wstring(*TypePointerStore) + L">" :
								   std::wstring(((MenuConsoleSizeStruct->Columns / 2) - EntryName.length() / 2), ' ') + EntryName + std::wstring(4, ' ') + L"" + std::to_wstring(*TypePointerStore) + L"");
			output += std::wstring(max((MenuConsoleSizeStruct->Columns - (output.size() + ((MenuConsoleSizeStruct->Columns / 2) - output.size() / 2))), 0), L' ') + L'\n';
			return output;
		}

		/// <summary>
		/// pass/send a input to the Entry
		/// </summary>
		/// <param name="inputType">- input that is getting sent</param>
		void MenuEntry<int>::EntryInput(EntryInputPassStruct* inputStruct)
		{
			switch (inputStruct->inputType)
			{
			case EntryInputPassStruct::InputType::Enter:
			{
				std::wstring NewInt;
				wchar_t ch;
				bool ContinueIntType = true;

				COORD NumberPosition = { (((MenuConsoleSizeStruct->Columns / 2) - EntryName.length() / 2) + EntryName.length() + 5), (inputStruct->CurrentIndex + inputStruct->TitleSize) };

				SetConsoleCursorPosition(*MenuConsoleHandle, NumberPosition);
				NosStdLib::Global::Console::ShowCaret(true);

				while (ContinueIntType)
				{
					ch = _getch();

					if (ch == ENTER)
					{
						ContinueIntType = false;
					}
					else if (ch == BACKSPACE)
					{
						// Coord for backspace cursor position editing
						COORD NewCoord;

						if (!NewInt.empty())
						{
							*MenuConsoleSizeStruct = NosStdLib::Global::Console::GetConsoleSize(*MenuConsoleHandle, MenuConsoleScreenBI);

							NewCoord = { (SHORT)(MenuConsoleScreenBI->dwCursorPosition.X - 1), MenuConsoleScreenBI->dwCursorPosition.Y }; // create new coord with x-1 and same y
							SetConsoleCursorPosition(*MenuConsoleHandle, NewCoord); // use new coord
							wprintf(L" "); // delete character
							SetConsoleCursorPosition(*MenuConsoleHandle, NewCoord);
							NewInt.pop_back();
						}
					}
					else if (isdigit(ch) || (ch == '-' && NewInt.empty()))
					{
						wprintf(L"%c", ch);
						NewInt += ch;
					}
				}

				if (!NewInt.empty())
				{
					try
					{
						*TypePointerStore = std::stoi(NewInt);
					}
					catch (const std::out_of_range& ex)
					{
						if (NewInt[0] == '-')
							*TypePointerStore = INT_MIN;
						else
							*TypePointerStore = INT_MAX;
					}
					catch (...)
					{
						/* if any other exception, do nothing */
					}
				}

				SetConsoleCursorPosition(*MenuConsoleHandle, { 0, (SHORT)(inputStruct->CurrentIndex + inputStruct->TitleSize) });
				wprintf(EntryString(true).c_str());
				NosStdLib::Global::Console::ShowCaret(false); /* hide the caret again */
				break;
			}
			case EntryInputPassStruct::InputType::ArrowLeft:
				(*TypePointerStore)--;
				SetConsoleCursorPosition(*MenuConsoleHandle, { 0, (SHORT)(inputStruct->CurrentIndex + inputStruct->TitleSize) });
				wprintf(EntryString(true).c_str());
				break;
			case EntryInputPassStruct::InputType::ArrowRight:
				(*TypePointerStore)++;
				SetConsoleCursorPosition(*MenuConsoleHandle, { 0, (SHORT)(inputStruct->CurrentIndex + inputStruct->TitleSize) });
				wprintf(EntryString(true).c_str());
				break;
			}
		}
	#pragma endregion

	#pragma region DynamicMenu
		/// <summary>
		/// create a wstring which shows the Entry name, value and is also centered
		/// </summary>
		/// <param name="selected">- if the entry is selected or not</param>
		/// <returns>wstring which shows the Entry name, value and is also centered</returns>
		std::wstring MenuEntry<DynamicMenu>::EntryString(bool selected)
		{
			*MenuConsoleSizeStruct = NosStdLib::Global::Console::GetConsoleSize(*MenuConsoleHandle, MenuConsoleScreenBI); /* Update values */

			std::wstring output = (selected ? 
								   std::wstring(((MenuConsoleSizeStruct->Columns / 2) - EntryName.length() / 2) - 3, ' ') + NosStdLib::TextColor::NosRGB(212, 155, 55).MakeANSICode<wchar_t>() + L">> " + EntryName + L" <<" :
								   std::wstring(((MenuConsoleSizeStruct->Columns / 2) - EntryName.length() / 2) - 0, ' ') + NosStdLib::TextColor::NosRGB(212, 155, 55).MakeANSICode<wchar_t>() + L"" + EntryName + L"");
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
				NosStdLib::Global::Console::ClearScreen();
				(*TypePointerStore).StartMenu();
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