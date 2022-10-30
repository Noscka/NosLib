#ifndef _DYNAMICMENUSYSTEM_HPP_
#define _DYNAMICMENUSYSTEM_HPP_

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

		class MenuEntryBase;

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
			virtual void EntryInput(const EntryInputPassStruct& inputStruct)
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
					std::wstring output = std::wstring(SpaceLenght - (selected ? 3 : 0) , ' ') + (selected ? L">> " : L"") + EntryName + (selected ? L" <<" : L"");
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
			void EntryInput(const EntryInputPassStruct& inputStruct)
			{
				if constexpr (std::is_base_of_v<NosStdLib::Functional::FunctionStoreBase, EntryType>)
				{
					switch (inputStruct.inputType)
					{
					case EntryInputPassStruct::InputType::Enter:
						TypePointerStore->RunFunction(); /* TODO: Figure out how to make it clear screen and then redraw menu */
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

			std::wstring output = std::wstring(((MenuConsoleSizeStruct->Columns / 2) - EntryName.length() / 2) - (selected ? 3 : 0), ' ') + (selected ? L">> " : L"") + EntryName + std::wstring(4, ' ') + (*TypePointerStore ? L"[X]" : L"[ ]") + (selected ? L" <<" : L"");
			output += std::wstring(max((MenuConsoleSizeStruct->Columns - (output.size() + ((MenuConsoleSizeStruct->Columns / 2) - output.size() / 2))), 0), L' ') + L'\n';
			return output;
		}

		/// <summary>
		/// pass/send a input to the Entry
		/// </summary>
		/// <param name="inputType">- input that is getting sent</param>
		void MenuEntry<bool>::EntryInput(const EntryInputPassStruct& inputStruct)
		{
			switch (inputStruct.inputType)
			{
			case EntryInputPassStruct::InputType::Enter:
				(*TypePointerStore) = !(*TypePointerStore); /* TODO: Redraw current line too */
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

			std::wstring output = std::wstring(((MenuConsoleSizeStruct->Columns / 2) - EntryName.length() / 2), ' ') + EntryName + std::wstring(4, ' ') + (selected ? L"<" : L"") + std::to_wstring(*TypePointerStore) + (selected ? L">" : L"");
			output += std::wstring(max((MenuConsoleSizeStruct->Columns - (output.size() + ((MenuConsoleSizeStruct->Columns / 2) - output.size() / 2))), 0), L' ') + L'\n';
			return output;
		}

		/// <summary>
		/// pass/send a input to the Entry
		/// </summary>
		/// <param name="inputType">- input that is getting sent</param>
		void MenuEntry<int>::EntryInput(const EntryInputPassStruct& inputStruct)
		{
			switch (inputStruct.inputType)
			{
			case EntryInputPassStruct::InputType::Enter:
			{
				std::wstring NewInt;
				wchar_t ch;
				bool ContinueIntType = true;

				COORD NumberPosition = { (((MenuConsoleSizeStruct->Columns / 2) - EntryName.length() / 2) + EntryName.length() + 5), (inputStruct.CurrentIndex + inputStruct.TitleSize) };

				SetConsoleCursorPosition(*MenuConsoleHandle, NumberPosition);

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
							GetConsoleScreenBufferInfo(MenuConsoleHandle, MenuConsoleScreenBI);

							NewCoord = { (SHORT)(MenuConsoleScreenBI->dwCursorPosition.X - 1), MenuConsoleScreenBI->dwCursorPosition.Y }; // create new coord with x-1 and same y
							SetConsoleCursorPosition(MenuConsoleHandle, NewCoord); // use new coord
							wprintf(L" "); // delete character
							SetConsoleCursorPosition(MenuConsoleHandle, NewCoord);
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
					catch (std::out_of_range ex)
					{
						if (NewInt[0] == '-')
							*TypePointerStore = INT_MIN;
						else
							*TypePointerStore = INT_MAX;
					}
				}
				SetConsoleCursorPosition(MenuConsoleHandle, {0, (SHORT)(inputStruct.CurrentIndex + inputStruct.TitleSize) }); /* TODO: Figure out why this doesn't overwrite previous */
				wprintf(EntryString(true).c_str());
				break;
			}
			case EntryInputPassStruct::InputType::ArrowLeft:
				break;
			case EntryInputPassStruct::InputType::ArrowRight:
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

			std::wstring output = std::wstring(((MenuConsoleSizeStruct->Columns / 2) - EntryName.length() / 2) - (selected ? 3 : 0), ' ') + NosStdLib::TextColor::MakeANSICode<wchar_t>(NosStdLib::TextColor::NosRGB(212, 155, 55)) + (selected ? L">> " : L"") + EntryName + (selected ? L" <<" : L"");
			output += std::wstring(max((MenuConsoleSizeStruct->Columns - (output.size() + ((MenuConsoleSizeStruct->Columns / 2) - output.size() / 2))), 0), L' ') + L"\033[0m\n";
			return output;
		}

		/// <summary>
		/// pass/send a input to the Entry
		/// </summary>
		/// <param name="inputType">- input that is getting sent</param>
		void MenuEntry<DynamicMenu>::EntryInput(const EntryInputPassStruct& inputStruct)
		{
			switch (inputStruct.inputType)
			{
			case EntryInputPassStruct::InputType::Enter:

				break;
			case EntryInputPassStruct::InputType::ArrowLeft:
				break;
			case EntryInputPassStruct::InputType::ArrowRight:
				break;
			}
		}
	#pragma endregion
	#pragma endregion

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
					AddedQuit = true;
					std::function<void()> Func = [this]()
					{
						return this->QuitMenu();
					};
					//MenuEntryList.Append(new MenuEntry(L"Quit", new NosStdLib::Functional::FunctionStore(&std::bind(&DynamicMenu::QuitMenu, this)))); TODO: add back adding the quit menu
				}

				int ch, exCh; /* for getting input data */
				int currentIndex = 0; /* Which item is currently selected */
				int oldIndex = currentIndex; /* Old index to know old position */
				int titleSize = 0; /* title size (for calculations where actual menu entries start) */
				int lastMenuSize = MenuEntryList.GetArrayIndexPointer(); /* for checking if the menu has increased/descreased */
				ConsoleSizeStruct = NosStdLib::Global::Console::GetConsoleSize(ConsoleHandle, &ConsoleScreenBI); /* Update the ConsoleSize first time */

				DrawMenu(currentIndex, &titleSize); /* Draw menu first time */

				while (MenuLoop)
				{
					ch = _getch(); /* first character input */
					if (ch == ENTER)
					{
						MenuEntryList[currentIndex]->EntryInput({ currentIndex, titleSize, EntryInputPassStruct::InputType::Enter });
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
							MenuEntryList[currentIndex]->EntryInput({ currentIndex, titleSize, EntryInputPassStruct::InputType::ArrowLeft });
							break;
						case ARROW_RIGHT:
							MenuEntryList[currentIndex]->EntryInput({ currentIndex, titleSize, EntryInputPassStruct::InputType::ArrowRight });
							break;
						}
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

					COORD finalPosition;

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


					//DrawMenu(currentIndex, &titleSize); /* Draw menu first time */
					oldIndex = currentIndex;
				}
				NosStdLib::Global::Console::ClearScreen(); /* Clear the screen to remove the menu */
			}

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
			/// Adds entry to menu
			/// </summary>
			/// <param name="Entry">- the entry to add</param>
			void AddMenuEntry(MenuEntryBase* Entry)
			{
				Entry->SetEntryVariables(&ConsoleHandle, &ConsoleScreenBI, &ConsoleSizeStruct);
				MenuEntryList.Append(Entry);
			}

			/// <summary>
			/// quits the menu
			/// </summary>
			void QuitMenu()
			{
				MenuLoop = false;
			}
		};
	}
}

#endif