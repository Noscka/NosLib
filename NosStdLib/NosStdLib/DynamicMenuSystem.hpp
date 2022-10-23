#ifndef _DYNAMICMENUSYSTEM_HPP_
#define _DYNAMICMENUSYSTEM_HPP_

#include <Windows.h>
#include <string>
#include <functional>

#include "DynamicArray.hpp"

#define ARROW_UP    72
#define ARROW_DOWN  80
#define ARROW_LEFT	75
#define ARROW_RIGHT	77

#define ENTER       13
#define BACKSPACE	8

namespace NosStdLib
{
	class DynamicMenu;

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

	class DynamicMenu
	{
	private:
		CONSOLE_SCREEN_BUFFER_INFO csbi;
		HANDLE ConsoleHandle;
		int columns, rows;
		DynamicArray<MenuEntry> MenuEntryList;
		bool ContinueMenu, AddExitEntry, CustomTitle, CenteredTitle, AddedQuit;
		std::wstring Title;
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

			MenuEntryList = DynamicArray<MenuEntry>(3, 2);
			ConsoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
		}

		/// <summary>
		/// Starts the menu
		/// </summary>
		void StartMenu();

		/// <summary>
		/// Draws the menu
		/// </summary>
		/// <param name="CurrentIndex">- currrent index</param>
		/// <param name="TitleSize">- pointer to the title size int so it can be calculated</param>
		void DrawMenu(int CurrentIndex, int* TitleSize);

		/// <summary>
		/// Returns line string for a entry
		/// </summary>
		/// <param name="EntryIndex">- the index of the entry wanted</param>
		/// <param name="selected">- if the entry is selected or not</param>
		/// <returns>Entry line string</returns>
		std::wstring EntryString(int EntryIndex, bool selected);

		/// <summary>
		/// Clears the whole screen
		/// </summary>
		/// <param name="fill">- what character to fill the screen with</param>
		void clear_screen(char fill = ' ');

		/// <summary>
		/// Clears the line selected
		/// </summary>
		/// <param name="Position">- The absolute position</param>
		void ClearCurrentLine(int Position);

		/// <summary>
		/// Adds entry to menu
		/// </summary>
		/// <param name="Entry">- the entry to add</param>
		void AddMenuEntry(MenuEntry Entry);

		/// <summary>
		/// quits the menu
		/// </summary>
		void QuitMenu();
	};
}

#endif