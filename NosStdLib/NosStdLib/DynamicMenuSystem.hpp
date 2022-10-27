#ifndef _DYNAMICMENUSYSTEM_HPP_
#define _DYNAMICMENUSYSTEM_HPP_

#include "Global.hpp"
#include "DynamicArray.hpp"
#include "UnicodeTextGenerator.hpp"

#include <Windows.h>
#include <conio.h>
#include <string>
#include <functional>
#include <type_traits>

/* TODO: Rewrite DynamicMenuSystem as it seems to be frequently crashing due to memory issues.
Features needed:
 - dynamic functions with parameter passthrough (menu entry).
 - text overwrite algorithm instead of clear and write algorithm.
*/

namespace NosStdLib
{
	namespace MenuRewrite
	{

		/// <summary>
		/// Class which is used for entries in the DynamicMenu class, contains the necessary data
		/// </summary>
		template <typename EntryType>
		class MenuEntry
		{
			//static_assert(std::is_same_v<int, EntryType>, "MenuEntry has to be int, bool,"); /* Create statis Assert to stop user from creating objects with types that aren't allowed */
		private:
			std::wstring EntryName;

			EntryType* EntryStoreObjectPointer;
		public:
			/// <summary>
			/// Create empty entry
			/// </summary>
			MenuEntry(){}

			/// <summary>
			/// Create entry with an object
			/// </summary>
			/// <param name="entryName">- the name that will be showed in the menu</param>
			/// <param name="entryStoreObjectPointer">- a pointer to the object which will get used</param>
			MenuEntry(std::wstring entryName, EntryType* entryStoreObjectPointer)
			{
				EntryName = entryName;
				EntryStoreObjectPointer = entryStoreObjectPointer
			}

		};

		/// <summary>
		/// the main Menu class which will be used to 
		/// </summary>
		class DynamicMenu
		{

		};

	}

	namespace Menu
	{
		constexpr int ARROW_UP		=	72;
		constexpr int ARROW_DOWN	=	80;
		constexpr int ARROW_LEFT	=	75;
		constexpr int ARROW_RIGHT	=	77;

		constexpr int ENTER			=	13;
		constexpr int BACKSPACE		=	8;

		class DynamicMenu;

		/// <summary>
		/// Class which demostrates an entry in the menu
		/// </summary>
		class MenuEntry
		{
		public:
			enum Type
			{
				EmptyEntry = 0, /* Entry with just name */
				FunctionEntry = 1, /* Normal entry with a function */
				SubMenuEntry = 2, /* an entry which contains a submenu */
				BooleanEntry = 3, /* an entry that has a boolean */
				IntegerEntry = 4, /* an entry that has a Integer */
			};

			// Var across all types
			std::wstring Name;
			Type EntryType;

			// Type specific vars
			std::function<void()> Function; /* TODO: allow any function type */
			DynamicMenu* SubMenu;
			bool* Boolean;
			int* Integer;

			/// <summary>
			/// For Arrays
			/// </summary>
			MenuEntry() {}

			/// <summary>
			/// Empty Entry
			/// </summary>
			/// <param name="name">- Seperator characters</param>
			MenuEntry(std::wstring name)
			{
				Name = name;
				EntryType = EmptyEntry;
			}

			/// <summary>
			/// Function Entry
			/// </summary>
			/// <param name="name">- entry name</param>
			/// <param name="function">- what to do when selected</param>
			MenuEntry(std::wstring name, std::function<void()> function)
			{
				Name = name;
				Function = function;
				EntryType = FunctionEntry;
			}

			/// <summary>
			/// Sub Menu entry
			/// </summary>
			/// <param name="name">- entry name</param>
			/// <param name="subMenu">- pointer to submenu object</param>
			MenuEntry(std::wstring name, DynamicMenu* subMenu)
			{
				Name = name;
				SubMenu = subMenu;
				EntryType = SubMenuEntry;
			}

			/// <summary>
			/// Boolean entry
			/// </summary>
			/// <param name="name">- entry name</param>
			/// <param name="boolean">- pointer to bool</param>
			MenuEntry(std::wstring name, bool* boolean)
			{
				Name = name;
				Boolean = boolean;
				EntryType = BooleanEntry;
			}

			/// <summary>
			/// integer entry
			/// </summary>
			/// <param name="name">- entry name</param>
			/// <param name="integer">- pointer to int</param>
			MenuEntry(std::wstring name, int* integer)
			{
				Name = name;
				Integer = integer;
				EntryType = IntegerEntry;
			}
		};

		/// <summary>
		/// Class for the Dynamic menu
		/// </summary>
		class DynamicMenu
		{
		private:
			std::wstring Title;														/* The title */
			CONSOLE_SCREEN_BUFFER_INFO csbi;										/* global ConsoleScreenBI so it is synced across all operations */
			HANDLE ConsoleHandle;													/* global Console Handle so it is synced across all operations and so it doesn't have to retrieved */
			int Columns, Rows;														/* global Colums and Rows, currently gets recalculated in some functions to keep updated */
			NosStdLib::DynamicArray<MenuEntry*> MenuEntryList;						/* Menu Entry pointer dynamicArray to store all the menu entries */
			bool ContinueMenu,	/* bool if the menu should continue or if it should quit */
				AddExitEntry,	/* bool if to add a quit option/entry at the bottom */
				CustomTitle,	/* bool if the menu has a custom menu */
				CenteredTitle,	/* bool if the menu should center the title */
				AddedQuit;		/* bool on if the quit option/entry was added or not */
		public:

			/// <summary>
			/// creates the menu object
			/// </summary>
			/// <param name="title">- menu title</param>
			/// <param name="customTitle">(default = false) - if the title should be generated to UnicodeText or use the string direct</param>
			/// <param name="addExitEntry">(default = true) - add a premade function to stop the menu</param>
			/// <param name="centeredTitle">(default = true) - If the title should be centered or not</param>
			DynamicMenu(std::wstring title, bool customTitle = false, bool addExitEntry = true, bool centeredTitle = true)
			{
				Title = title;
				AddExitEntry = addExitEntry;
				CustomTitle = customTitle;
				CenteredTitle = centeredTitle;

				AddedQuit = false;

				MenuEntryList = NosStdLib::DynamicArray<MenuEntry*>(3, 3);
				ConsoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
			}

			/// <summary>
			/// Starts the menu
			/// </summary>
			void StartMenu()
			{
				ContinueMenu = true; // incase menu was quit before

				if (AddExitEntry && !AddedQuit)
				{
					AddedQuit = true;
					std::function<void()> Func = [this]()
					{
						return this->QuitMenu();
					};

					MenuEntryList.Append(new MenuEntry(L"Quit", Func));
				}

				int c, ex;
				int CurrentIndex = 0; /* Which item is currently selected */
				int OldIndex = CurrentIndex; /* Old index to know old position */
				int TitleSize = 0; /* title size (for calculations where actual menu entries start) */

				int LastMenuSize = MenuEntryList.GetArrayIndexPointer(); /* for checking if the menu has increased/descreased */

				DrawMenu(CurrentIndex, &TitleSize); /* Draw menu first time*/

				while (ContinueMenu)
				{
					// get key input for arrow and enter inputs
					c = _getch();

					if (c == ENTER)
					{

						switch (MenuEntryList[CurrentIndex]->EntryType)
						{
						case MenuEntry::Type::FunctionEntry:
							NosStdLib::Global::Console::ClearScreen();
							MenuEntryList[CurrentIndex]->Function();
							DrawMenu(CurrentIndex, &TitleSize);
							break;

						case MenuEntry::Type::SubMenuEntry:
							NosStdLib::Global::Console::ClearScreen();
							MenuEntryList[CurrentIndex]->SubMenu->StartMenu();
							DrawMenu(CurrentIndex, &TitleSize);
							break;

						case MenuEntry::Type::BooleanEntry:
							*MenuEntryList[CurrentIndex]->Boolean = !*MenuEntryList[CurrentIndex]->Boolean;

							NosStdLib::Global::Console::ClearLine(TitleSize + CurrentIndex);

							wprintf(EntryString(CurrentIndex, true).c_str());
							break;

						case MenuEntry::Type::IntegerEntry:
						{
							std::wstring NewInt;
							wchar_t c;
							bool ContinueIntType = true;

							COORD NumberPosition = { (((Columns / 2) - MenuEntryList[CurrentIndex]->Name.length() / 2) + MenuEntryList[CurrentIndex]->Name.length() + 3), (CurrentIndex + TitleSize) };

							SetConsoleCursorPosition(ConsoleHandle, NumberPosition);

							while (ContinueIntType)
							{
								c = _getch();

								if (c == ENTER)
								{
									ContinueIntType = false;
								}
								else if (c == BACKSPACE)
								{
									// Coord for backspace cursor position editing
									COORD NewCoord;

									if (!NewInt.empty())
									{
										GetConsoleScreenBufferInfo(ConsoleHandle, &csbi);

										NewCoord = { (SHORT)(csbi.dwCursorPosition.X - 1), csbi.dwCursorPosition.Y }; // create new coord with x-1 and same y
										SetConsoleCursorPosition(ConsoleHandle, NewCoord); // use new coord
										wprintf(L" "); // delete character
										SetConsoleCursorPosition(ConsoleHandle, NewCoord);
										NewInt.pop_back();
									}
								}
								else if (isdigit(c) || (c == '-' && NewInt.empty()))
								{
									wprintf(L"%c", c);
									NewInt += c;
								}
							}

							if (!NewInt.empty())
							{
								try
								{
									*MenuEntryList[CurrentIndex]->Integer = std::stoi(NewInt);
								}
								catch (std::out_of_range ex)
								{
									if (NewInt[0] == '-')
										*MenuEntryList[CurrentIndex]->Integer = INT_MIN;
									else
										*MenuEntryList[CurrentIndex]->Integer = INT_MAX;
								}
							}
							NosStdLib::Global::Console::ClearLine(TitleSize + CurrentIndex);
							wprintf(EntryString(CurrentIndex, true).c_str());
							break;
						}

						default:
							break;
						}
					}
					else if (!(c && c != 224))
					{
						// if not enter, do second getch as arrows input 2 characters to show they are arrows
						switch (ex = _getch())
						{
						case ARROW_UP:
							if (CurrentIndex > 0) // Decrement only if smaller then List size
							{
								CurrentIndex--; // Decrement the Indenetation
							}
							break;

						case ARROW_DOWN:
							if (CurrentIndex < MenuEntryList.GetArrayIndexPointer() - 1) // Increment only if larger the 0
							{
								CurrentIndex++; // Increment the Indenetation
							}
							break;

						case ARROW_LEFT:
							if (MenuEntryList[CurrentIndex]->EntryType == MenuEntry::Type::IntegerEntry)
							{
								(*MenuEntryList[CurrentIndex]->Integer)--;

								NosStdLib::Global::Console::ClearLine(TitleSize + CurrentIndex);
								wprintf(EntryString(CurrentIndex, true).c_str());
							}
							break;

						case ARROW_RIGHT:
							if (MenuEntryList[CurrentIndex]->EntryType == MenuEntry::Type::IntegerEntry)
							{
								(*MenuEntryList[CurrentIndex]->Integer)++;

								NosStdLib::Global::Console::ClearLine(TitleSize + CurrentIndex);
								wprintf(EntryString(CurrentIndex, true).c_str());
							}
							break;
						}
					}

					if (LastMenuSize != MenuEntryList.GetArrayIndexPointer())
					{
						DrawMenu(CurrentIndex, &TitleSize); /* Redraw menu to include new entries */
						LastMenuSize = MenuEntryList.GetArrayIndexPointer();
					}
					else if (CurrentIndex == OldIndex)
						continue;
					else
					{
						COORD tl;
						if (CurrentIndex > OldIndex)
							tl = { 0, (SHORT)(TitleSize + CurrentIndex - 1) };
						else
							tl = { 0, (SHORT)(TitleSize + CurrentIndex) };

						GetConsoleScreenBufferInfo(ConsoleHandle, &csbi);
						DWORD written, cells = csbi.dwSize.X * 2;
						FillConsoleOutputCharacter(ConsoleHandle, ' ', cells, tl, &written);
						FillConsoleOutputAttribute(ConsoleHandle, csbi.wAttributes, cells, tl, &written);

						/*
						What needs to be redrawing depending on if its up for down
						if the index goes down (bigger number), you need to clear above and current line

						|| Old Selected Entry
						\/ New Selected Entry <-- Here is Cursor

						and if going up (smaller number)
						/\ New Selected Entry <-- Here is Cursor
						|| Old Selected Entry
						*/

						COORD FinalPosition;

						if (CurrentIndex > OldIndex) /* Going Down */
						{

							SetConsoleCursorPosition(ConsoleHandle, { 0, (SHORT)(TitleSize + CurrentIndex - 1) });
							wprintf((EntryString(OldIndex, false) + EntryString(CurrentIndex, true)).c_str());

							if ((TitleSize + CurrentIndex) + (Rows / 2) < 0)
								FinalPosition = { 0,0 };
							else if ((TitleSize + CurrentIndex) + (Rows / 2) > MenuEntryList.GetArrayIndexPointer())
								FinalPosition = { 0, (SHORT)(MenuEntryList.GetArrayIndexPointer() + TitleSize - 1) };
							else
								FinalPosition = { 0, (SHORT)((TitleSize + CurrentIndex) + (Rows / 2)) };

							SetConsoleCursorPosition(ConsoleHandle, FinalPosition);
						}
						else /* Going Up */
						{
							SetConsoleCursorPosition(ConsoleHandle, { 0, (SHORT)(TitleSize + CurrentIndex) });
							wprintf((EntryString(CurrentIndex, true) + EntryString(OldIndex, false)).c_str());

							if ((TitleSize + CurrentIndex) - (Rows / 2) < 0)
								FinalPosition = { 0,0 };
							else if ((TitleSize + CurrentIndex) - (Rows / 2) > MenuEntryList.GetArrayIndexPointer())
								FinalPosition = { 0, (SHORT)MenuEntryList.GetArrayIndexPointer() };
							else
								FinalPosition = { 0, (SHORT)((TitleSize + CurrentIndex) - (Rows / 2)) };

							SetConsoleCursorPosition(ConsoleHandle, FinalPosition);
						}

					}

					OldIndex = CurrentIndex;
				}
				NosStdLib::Global::Console::ClearScreen(); /* Clear the screen to remove the menu */
			}

			/// <summary>
			/// Draws the menu
			/// </summary>
			/// <param name="CurrentIndex">- currrent index</param>
			/// <param name="TitleSize">- pointer to the title size int so it can be calculated</param>
			void DrawMenu(int CurrentIndex, int* TitleSize)
			{
				NosStdLib::Global::Console::ClearScreen();

				GetConsoleScreenBufferInfo(ConsoleHandle, &csbi);
				Columns = csbi.srWindow.Right - csbi.srWindow.Left + 1;
				Rows = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;

				std::wstring OutputString; // string for full "display" as it is the most perfomace efficent method

				if (CustomTitle) /* If custom Title is true, its going to use the straight characters instead of generating a unicode title */
					if (CenteredTitle)
						OutputString = Title;
					else
						OutputString = std::wstring(((Columns / 2) - Title.length() / 2), ' ') + Title;
				else
					OutputString = NosStdLib::UnicodeTextGenerator::BasicUnicodeTextGenerate(ConsoleHandle, Title, CenteredTitle); // add title with "ascii generator"

				*TitleSize = 0;

				for (int i = 0; i < OutputString.size(); i++)
				{
					if (OutputString[i] == '\n')
					{
						(*TitleSize)++;
					}
				}

				// for loop using counter to get the index so to add the >< to the selected option
				for (int i = 0; i < MenuEntryList.GetArrayIndexPointer(); i++)
				{
					if (i == CurrentIndex)
						OutputString += EntryString(i, true);
					else
						OutputString += EntryString(i, false);
				}

				wprintf(OutputString.c_str());

				SetConsoleCursorPosition(ConsoleHandle, { 0, (SHORT)(CurrentIndex) });
			}

			/// <summary>
			/// Returns line string for a entry
			/// </summary>
			/// <param name="EntryIndex">- the index of the entry wanted</param>
			/// <param name="selected">- if the entry is selected or not</param>
			/// <returns>Entry line string</returns>
			std::wstring EntryString(int EntryIndex, bool selected)
			{
				int SpaceLenght = ((Columns / 2) - MenuEntryList[EntryIndex]->Name.length() / 2);
				std::wstring EntryText;

				switch (MenuEntryList[EntryIndex]->EntryType) /* Different printing types for different entry types*/
				{
				case MenuEntry::Type::FunctionEntry:
					// Append to string as to make it be 1 print operation, makes it way quicker
					if (selected)
					{
						return std::wstring(SpaceLenght - 2, ' ') + L">>" + MenuEntryList[EntryIndex]->Name + L"<<\n";
					}
					else
					{
						return std::wstring(SpaceLenght, ' ') + MenuEntryList[EntryIndex]->Name + L"\n";
					}
					break;
				case MenuEntry::Type::SubMenuEntry:
					if (selected)
					{
						return std::wstring(SpaceLenght - 2, ' ') + L"\033[33m>>" + MenuEntryList[EntryIndex]->Name + L"<<\033[0m\n";
					}
					else
					{
						return std::wstring(SpaceLenght, ' ') + L"\033[33m" + MenuEntryList[EntryIndex]->Name + L"\033[0m\n";
					}
					break;

				case MenuEntry::Type::BooleanEntry:
					if (*MenuEntryList[EntryIndex]->Boolean)
						EntryText = MenuEntryList[EntryIndex]->Name + std::wstring(4, ' ') + L"[X]";
					else
						EntryText = MenuEntryList[EntryIndex]->Name + std::wstring(4, ' ') + L"[ ]";

					if (selected)
					{
						return std::wstring(SpaceLenght - 2, ' ') + L">>" + EntryText + L"<<\n";
					}
					else
					{
						return std::wstring(SpaceLenght, ' ') + EntryText + L"\n";
					}
					break;

				case MenuEntry::Type::IntegerEntry:
					if (selected)
					{
						EntryText = MenuEntryList[EntryIndex]->Name + std::wstring(4, ' ') + L"<" + std::to_wstring(*MenuEntryList[EntryIndex]->Integer) + L">\n";
						return std::wstring(SpaceLenght - 2, ' ') + EntryText;
					}
					else
					{
						EntryText = MenuEntryList[EntryIndex]->Name + std::wstring(4, ' ') + std::to_wstring(*MenuEntryList[EntryIndex]->Integer);
						return std::wstring(SpaceLenght, ' ') + EntryText + L"\n";
					}
					break;

				case MenuEntry::Type::EmptyEntry:
					return std::wstring(SpaceLenght, ' ') + MenuEntryList[EntryIndex]->Name + L"\n";
					break;
				}
			}

			/// <summary>
			/// Adds entry to menu
			/// </summary>
			/// <param name="Entry">- the entry to add</param>
			void AddMenuEntry(MenuEntry* Entry)
			{
				MenuEntryList.Append(Entry);
			}

			/// <summary>
			/// quits the menu
			/// </summary>
			void QuitMenu()
			{
				ContinueMenu = false;
			}
		};
	}
}

#endif