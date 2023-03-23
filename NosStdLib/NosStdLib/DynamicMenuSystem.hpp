#ifndef _DYNAMICMENUSYSTEM_NOSSTDLIB_HPP_
#define _DYNAMICMENUSYSTEM_NOSSTDLIB_HPP_

#include "Definitions.hpp"
#include "UnicodeTextGenerator.hpp"
#include "Console.hpp"
#include "RGB.hpp"
#include "Clickable.hpp"
#include "DynamicArray/ArrayPositionTrack.hpp"
#include "MouseTracking/MouseTracking.hpp"

#include <Windows.h>
#include <conio.h>
#include <chrono>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <queue>

namespace NosStdLib
{
	namespace Menu
	{
		enum MouseEventEnum : uint8_t
		{
			OnClick = 0,
			OnEnterHover = 1,
			OnLeaveHover = 2,
		};

		struct EntryStartAndLenght
		{
			int X1;
			int X2;
		};

		/* Define so MenuEntry can hold pointer */
		class DynamicMenu;

		struct EntryInputPassStruct
		{
			enum InputType : uint8_t
			{
				Enter = 0,
				ArrowLeft = 1,
				ArrowRight = 2,
			};

			int CurrentIndex;
			int TitleSize;
			DynamicMenu* ParentMenu;
			InputType inputType;
			bool Redraw;
		};


		/// <summary>
		/// The base of MenuEntry which only exists to allow for storing all the templated types together
		/// </summary>
		class MenuEntryBase : public NosStdLib::ArrayPositionTrack::PositionTrack, public NosStdLib::Clickable::Clickable
		{
		protected:
			std::wstring EntryName; /* The entry name */

			DynamicMenu* ParentMenuPointer;									/* a pointer to the menu which parents/holds this entry */
			HANDLE* MenuConsoleHandle;										/* a pointer to the menu's MenuConsoleHandle, so its synced */
			CONSOLE_SCREEN_BUFFER_INFO* MenuConsoleScreenBI;				/* a pointer to the menu's MenuConsoleScreenBI, so its synced */
			NosStdLib::Console::ConsoleSizeStruct* MenuConsoleSizeStruct;	/* a pointer to the menu's MenuConsoleSizeStruct, so its synced */
		public:
			/// <summary>
			/// create a wstring which shows the Entry name, value and is also centered
			/// </summary>
			/// <param name="selected">- if the entry is selected or not</param>
			/// <returns>wstring which shows the Entry name, value and is also centered</returns>
			virtual std::wstring EntryString(const bool& selected)
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
			/// Returns the X values the entry string stop and start positions
			/// </summary>
			/// <returns>2 X positions in a int,int struct</returns>
			virtual EntryStartAndLenght EntryStartAndLenghtPosition()
			{
				return {0,0};
			}


			/// <summary>
			/// Returns Entry name
			/// </summary>
			/// <returns>wstring of entry name</returns>
			std::wstring GetEntryName()
			{
				return EntryName;
			}

			/// <summary>
			/// Update/set pointers to ParentMenuPointer, ConsoleHandle, ConsoleScreenBI and ConsoleSizeStruct
			/// </summary>
			void SetEntryVariables(DynamicMenu* parentMenuPointer, HANDLE* menuConsoleHandle, CONSOLE_SCREEN_BUFFER_INFO* menuConsoleScreenBI, NosStdLib::Console::ConsoleSizeStruct* menuConsoleSizeStruct)
			{
				ParentMenuPointer = parentMenuPointer;
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

			/// <summary>
			/// Function to Set Mouse Events
			/// </summary>
			void SetMouseEvents();
		public:

			/// <summary>
			/// For Arrays
			/// </summary>
			MenuEntry() {}

			/// <summary>
			/// Function Entry
			/// </summary>
			/// <param name="name">- entry name</param>
			/// <param name="typePointerStore">- pointer to variable of type</param>
			MenuEntry(const std::wstring& name, EntryType* typePointerStore)
			{
				EntryName = name;
				TypePointerStore = typePointerStore;

				SetMouseEvents();
			}

			/// <summary>
			/// create a wstring which shows the Entry name, value and is also centered
			/// </summary>
			/// <param name="selected">- if the entry is selected or not</param>
			/// <returns>wstring which shows the Entry name, value and is also centered</returns>
			std::wstring EntryString(const bool& selected)
			{
				*MenuConsoleSizeStruct = NosStdLib::Console::GetConsoleSize(*MenuConsoleHandle, MenuConsoleScreenBI); /* Update values */

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
						NosStdLib::Console::ClearScreen();
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

			/// <summary>
			/// Retuns the X values the entry string stop and start positions
			/// </summary>
			/// <returns>2 X positions in a int,int struct</returns>
			EntryStartAndLenght EntryStartAndLenghtPosition()
			{
				*MenuConsoleSizeStruct = NosStdLib::Console::GetConsoleSize(*MenuConsoleHandle, MenuConsoleScreenBI); /* Update values */

				int x1 = ((MenuConsoleSizeStruct->Columns / 2) - (EntryName.length() / 2));

				return {x1, (int)(x1 + EntryName.length()-1)};
			}
		};

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
				 AddedQuit,				/* if quit entry was already added. TODO: store int of position and if more entries are added (last isn't quit), move quit to last */
				 ButtonStatus = false;	/* if the buttons are enabled or disabled currently */

			int TitleSize,				/* title size (for calculations where actual menu entries start) and also to create the clickable object boundries */
				CurrentIndex,			/* Which item is currently selected */
				OldIndex;				/* Old index to know old index position */

			std::queue<int> char_queue{};	/* input queue for the message loop */
		public:
			DynamicMenu(const std::wstring& title, const bool& generateUnicodeTitle = true, const bool& addExitEntry = true, const bool& centerTitle = true)
			{
				Title = title;
				AddExitEntry = addExitEntry;
				GenerateUnicodeTitle = generateUnicodeTitle;
				CenterTitle = centerTitle;

				ConsoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);

				UpdateTitleSize();
			}

			/// <summary>
			/// Starts the menu
			/// </summary>
			void StartMenu()
			{
				NosStdLib::MouseTracking::InitializeMouseTracking(); /* Initialize Mouse Tracking incase it was disabled */

				MenuLoop = true; /* incase menu was quit before */
				ButtonStatus = true; /* enable buttons for current menu */

				if (AddExitEntry && !AddedQuit)
				{
					AddMenuEntry(new MenuEntry(L"Quit", new NosStdLib::Functional::FunctionStore(&QuitMenu, this)));
					AddedQuit = true;
				}

				int ch, exCh; /* for getting input data */
				OldIndex = CurrentIndex = 0; /* Assign both old and current index ints */
				TitleSize = 0; /* title size (for calculations where actual menu entries start) */
				int lastMenuSize = MenuEntryList.GetArrayIndexPointer(); /* for checking if the menu has increased/descreased */
				ConsoleSizeStruct = NosStdLib::Console::GetConsoleSize(ConsoleHandle, &ConsoleScreenBI); /* Update the ConsoleSize first time */
				NosStdLib::Console::ConsoleSizeStruct oldConsoleSizeStruct = ConsoleSizeStruct;
				MSG msg;	/* MSG structer used for message loop */

				DrawMenu(); /* Draw menu first time */

				NosStdLib::Console::ShowCaret(false); /* Hide the caret */

				HANDLE eventHandle = CreateEvent(NULL, FALSE, FALSE, NULL);

				std::thread inputListenThread{[this, eventHandle]() {WaitForInput_Thread(eventHandle); }}; /* start waiting for input thread */

				while (MenuLoop) /* TODO: Make thread send message/event that can be used by the message loop */
				{
					switch (MsgWaitForMultipleObjects(1, &eventHandle, FALSE, 5, QS_ALLINPUT))
					{
					case WAIT_OBJECT_0 + 0: /* if event 0 (the input thread event) gets triggered */
						if (!char_queue.empty())
						{
							ch = char_queue.front();
							char_queue.pop();
							if (!(ch && ch != 224))
							{
								exCh = char_queue.front();
								char_queue.pop();
							}

							if (ch == NosStdLib::Definitions::ENTER)
							{ /* WARNING: Might need to show the caret again not mattering what EntryType it is, as for some functions. it might be necessary */
								NosStdLib::MouseTracking::TemporaryTerminateMouseTracking();
								ButtonStatus = false; /* temporarily disable buttons */
								EntryInputPassStruct InputPassStruct{ CurrentIndex, TitleSize, this, EntryInputPassStruct::InputType::Enter, false };
								MenuEntryList[CurrentIndex]->EntryInput(&InputPassStruct);
								if (InputPassStruct.Redraw)
								{
									DrawMenu();
								}
								NosStdLib::Console::ShowCaret(false); /* hide the caret again */
								ButtonStatus = true; /* re-enable buttons */
								NosStdLib::MouseTracking::InitializeMouseTracking();
							}
							else if (!(ch && ch != 224))
							{
								switch (exCh)
								{
								case NosStdLib::Definitions::ARROW_UP:
									if (CurrentIndex > 0) /* Decrement only if larger the 0 */
									{
										CurrentIndex--; /* Decrement the Indenetation */
									}
									break;
								case NosStdLib::Definitions::ARROW_DOWN:
									if (CurrentIndex < MenuEntryList.GetArrayIndexPointer() - 1) /* Increment only if smaller then List size */
									{
										CurrentIndex++; /* Increment the Indenetation */
									}
									break;
								case NosStdLib::Definitions::ARROW_LEFT:
									{
										EntryInputPassStruct InputPassStruct{ CurrentIndex, TitleSize,this, EntryInputPassStruct::InputType::ArrowLeft, false };
										MenuEntryList[CurrentIndex]->EntryInput(&InputPassStruct);
										if (InputPassStruct.Redraw)
											DrawMenu();
										break;
									}
								case NosStdLib::Definitions::ARROW_RIGHT:
									{
										EntryInputPassStruct InputPassStruct{ CurrentIndex, TitleSize,this, EntryInputPassStruct::InputType::ArrowRight, false };
										MenuEntryList[CurrentIndex]->EntryInput(&InputPassStruct);
										if (InputPassStruct.Redraw)
											DrawMenu();
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
								DrawMenu();
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

							if (CurrentIndex > OldIndex && OldIndex != CurrentIndex) /* Going Down */
							{
								SetConsoleCursorPosition(ConsoleHandle, { 0, (SHORT)(TitleSize + CurrentIndex - 1) });
								wprintf((MenuEntryList[OldIndex]->EntryString(false) + MenuEntryList[CurrentIndex]->EntryString(true)).c_str());

								if ((TitleSize + CurrentIndex) + (ConsoleSizeStruct.Rows / 2) < 0)
									finalPosition = { 0,0 };
								else if ((TitleSize + CurrentIndex) + (ConsoleSizeStruct.Rows / 2) > MenuEntryList.GetArrayIndexPointer())
									finalPosition = { 0, (SHORT)(MenuEntryList.GetArrayIndexPointer() + TitleSize - 1) };
								else
									finalPosition = { 0, (SHORT)((TitleSize + CurrentIndex) + (ConsoleSizeStruct.Rows / 2)) };
							}
							else if(OldIndex != CurrentIndex)/* Going Up */
							{
								SetConsoleCursorPosition(ConsoleHandle, { 0, (SHORT)(TitleSize + CurrentIndex) });
								wprintf((MenuEntryList[CurrentIndex]->EntryString(true) + MenuEntryList[OldIndex]->EntryString(false)).c_str());

								if ((TitleSize + CurrentIndex) - (ConsoleSizeStruct.Rows / 2) < 0)
									finalPosition = { 0,0 };
								else if ((TitleSize + CurrentIndex) - (ConsoleSizeStruct.Rows / 2) > MenuEntryList.GetArrayIndexPointer())
									finalPosition = { 0, (SHORT)MenuEntryList.GetArrayIndexPointer() };
								else
									finalPosition = { 0, (SHORT)((TitleSize + CurrentIndex) - (ConsoleSizeStruct.Rows / 2)) };

							}

							SetConsoleCursorPosition(ConsoleHandle, finalPosition);

							OldIndex = CurrentIndex;
						}
						break;

						case WAIT_OBJECT_0 + 1: /* if there is a message on the queue */
							PeekMessage(&msg, NULL, 0, 0, PM_REMOVE);
							TranslateMessage(&msg);
							DispatchMessage(&msg);
							break;

						case WAIT_TIMEOUT:
							break;
					}
				}
				ButtonStatus = false;	/* disable buttons for current menu */
				NosStdLib::Console::ClearScreen(); /* Clear the screen to remove the menu */
				NosStdLib::Console::ShowCaret(true); /* show the caret again */
				NosStdLib::MouseTracking::TerminateMouseTracking(); /* Terminate mouse tracking hook */
				inputListenThread.join(); /* wait for thread to join */
			}

			/// <summary>
			/// Function that gets called By MenuEntry object when it gets triggered by a mouse
			/// </summary>
			/// <param name="parentMenu">- pointer to parent menu</param>
			/// <param name="entryPosition">- array position of the entry</param>
			/// <param name="mouseOperationType">- the mouse event that happened</param>
			static void MouseEventCallback(DynamicMenu** parentMenu, int* entryPosition, const MouseEventEnum& mouseOperationType)
			{
				switch (mouseOperationType)
				{
				case MouseEventEnum::OnClick:
				{
					NosStdLib::MouseTracking::TemporaryTerminateMouseTracking();
					(*parentMenu)->ButtonStatus = false; /* temporarily disable buttons */
					(*parentMenu)->CurrentIndex = *entryPosition;
					EntryInputPassStruct InputPassStruct{(*parentMenu)->CurrentIndex, (*parentMenu)->TitleSize,*parentMenu, EntryInputPassStruct::InputType::Enter, false};
					(*parentMenu)->MenuEntryList[(*parentMenu)->CurrentIndex]->EntryInput(&InputPassStruct);
					if (InputPassStruct.Redraw)
					{
						(*parentMenu)->DrawMenu();
					}
					NosStdLib::Console::ShowCaret(false); /* hide the caret again */
					(*parentMenu)->ButtonStatus = true; /* re-enable button */
					NosStdLib::MouseTracking::InitializeMouseTracking();
					break;
				}

				case MouseEventEnum::OnEnterHover:
				{
					(*parentMenu)->CurrentIndex = *entryPosition;
					if ((*parentMenu)->OldIndex == (*parentMenu)->CurrentIndex) /* if old index and current index are equal */
					{
						return;
					}

					SetConsoleCursorPosition((*parentMenu)->ConsoleHandle, {0, (SHORT)((*parentMenu)->TitleSize + (*parentMenu)->OldIndex)});
					wprintf((*parentMenu)->MenuEntryList[(*parentMenu)->OldIndex]->EntryString(false).c_str());

					SetConsoleCursorPosition((*parentMenu)->ConsoleHandle, {0, (SHORT)((*parentMenu)->TitleSize + (*parentMenu)->CurrentIndex)});
					wprintf((*parentMenu)->MenuEntryList[(*parentMenu)->CurrentIndex]->EntryString(true).c_str());

					COORD finalPosition = {0,0};

					if ((*parentMenu)->CurrentIndex > (*parentMenu)->OldIndex) /* Going Down */
					{

						if (((*parentMenu)->TitleSize + (*parentMenu)->CurrentIndex) + ((*parentMenu)->ConsoleSizeStruct.Rows / 2) < 0)
							finalPosition = {0,0};
						else if (((*parentMenu)->TitleSize + (*parentMenu)->CurrentIndex) + ((*parentMenu)->ConsoleSizeStruct.Rows / 2) > (*parentMenu)->MenuEntryList.GetArrayIndexPointer())
							finalPosition = {0, (SHORT)((*parentMenu)->MenuEntryList.GetArrayIndexPointer() + (*parentMenu)->TitleSize - 1)};
						else
							finalPosition = {0, (SHORT)(((*parentMenu)->TitleSize + (*parentMenu)->CurrentIndex) + ((*parentMenu)->ConsoleSizeStruct.Rows / 2))};
					}
					else /* Going Up */
					{
						if (((*parentMenu)->TitleSize + (*parentMenu)->CurrentIndex) - ((*parentMenu)->ConsoleSizeStruct.Rows / 2) < 0)
							finalPosition = {0,0};
						else if (((*parentMenu)->TitleSize + (*parentMenu)->CurrentIndex) - ((*parentMenu)->ConsoleSizeStruct.Rows / 2) > (*parentMenu)->MenuEntryList.GetArrayIndexPointer())
							finalPosition = {0, (SHORT)(*parentMenu)->MenuEntryList.GetArrayIndexPointer()};
						else
							finalPosition = {0, (SHORT)(((*parentMenu)->TitleSize + (*parentMenu)->CurrentIndex) - ((*parentMenu)->ConsoleSizeStruct.Rows / 2))};
					}

					SetConsoleCursorPosition((*parentMenu)->ConsoleHandle, finalPosition);

					(*parentMenu)->OldIndex = (*parentMenu)->CurrentIndex;
					break;
				}

				case MouseEventEnum::OnLeaveHover:
					break;

				default:
					throw std::exception("Mouse Event not added");
					break;
				}
			}

			/// <summary>
			/// Adds entry to menu
			/// </summary>
			/// <param name="Entry">- the entry to add</param>
			void AddMenuEntry(MenuEntryBase* Entry)
			{
				Entry->SetEntryVariables(this, &ConsoleHandle, &ConsoleScreenBI, &ConsoleSizeStruct);
				EntryStartAndLenght xxValue = Entry->EntryStartAndLenghtPosition();
				Entry->ModifyClickablePosition(NosStdLib::Dimention::DimentionsD2(xxValue.X1, (TitleSize + MenuEntryList.GetArrayIndexPointer()), xxValue.X2, (TitleSize + MenuEntryList.GetArrayIndexPointer())));
				Entry->ModifyEnableBool(&ButtonStatus);
				MenuEntryList.Append(Entry);
			}

		private:
			/// <summary>
			/// Function used in a thread for waiting for input
			/// </summary>
			/// <param name="eventHandle">- handle of event to call when got input</param>
			void WaitForInput_Thread(const HANDLE& eventHandle)
			{
				int ch, exCh;
				while (MenuLoop)
				{
					if (_kbhit()) /* check if there is any input to take in */
					{
						ch = _getch();
						char_queue.push(ch);
						if (!(ch && ch != 224)) /* if the input WAS equal to 224, then wait for second input */
						{
							exCh = _getch();
							char_queue.push(exCh);
						}
						SetEvent(eventHandle); /* tell message loop that there are keys in queue */
					}

					Sleep(10); /* cool down so it doesn't check 8905925157028157085 times per millisecond */
				}
				return;
			}

			/// <summary>
			/// Draws the menu
			/// </summary>
			/// <param name="CurrentIndex">- currrent index</param>
			void DrawMenu()
			{
				NosStdLib::Console::ClearScreen();

				ConsoleSizeStruct = NosStdLib::Console::GetConsoleSize(ConsoleHandle, &ConsoleScreenBI);

				std::wstring outputString; /* string for full "display" as it is the most perfomace efficent method */

				if (GenerateUnicodeTitle) /* If custom Title is true, its going to use the straight characters instead of generating a unicode title */
				{
					outputString = NosStdLib::UnicodeTextGenerator::BasicUnicodeTextGenerate(ConsoleHandle, Title, CenterTitle); // add title with "ascii generator"
				}
				else
				{
					if (CenterTitle) {outputString = std::wstring(((ConsoleSizeStruct.Columns / 2) - Title.length() / 2), ' ') + Title;}
					else {outputString = Title;}
				}

				UpdateTitleSize();

				// for loop using counter to get the index so to add the >< to the selected option
				for (int i = 0; i < MenuEntryList.GetArrayIndexPointer(); i++)
				{
					if (i == CurrentIndex)
					{
						outputString += MenuEntryList[i]->EntryString(true);
					}
					else
					{
						outputString += MenuEntryList[i]->EntryString(false);
					}
				}

				wprintf(outputString.c_str());

				SetConsoleCursorPosition(ConsoleHandle, { 0, (SHORT)(CurrentIndex) });
			}

			/// <summary>
			/// Calculate and update the TitleSize int variable
			/// </summary>
			void UpdateTitleSize()
			{
				if (GenerateUnicodeTitle)
				{
					TitleSize = 6;
				}
				else
				{
					TitleSize = std::count(Title.begin(), Title.end(), L'\n');
				}
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


		/* Needs to be down here so it has "MouseEventCallback" function already declared and defined */
		template<class EntryType>
		void MenuEntry<EntryType>::SetMouseEvents()
		{
			/* Setup Event */
			OnClick = new Event(new NosStdLib::Functional::FunctionStore(&DynamicMenu::MouseEventCallback, &ParentMenuPointer, this->GetArrayPositionPointer(), MouseEventEnum::OnClick));
			OnEnterHover = new Event(new NosStdLib::Functional::FunctionStore(&DynamicMenu::MouseEventCallback, &ParentMenuPointer, this->GetArrayPositionPointer(), MouseEventEnum::OnEnterHover));
			OnLeaveHover = new Event(new NosStdLib::Functional::FunctionStore(&DynamicMenu::MouseEventCallback, &ParentMenuPointer, this->GetArrayPositionPointer(), MouseEventEnum::OnLeaveHover));
		}

	#pragma region Template Specialization
	#pragma region bool
	/// <summary>
	/// create a wstring which shows the Entry name, value and is also centered
	/// </summary>
	/// <param name="selected">- if the entry is selected or not</param>
	/// <returns>wstring which shows the Entry name, value and is also centered</returns>
		std::wstring MenuEntry<bool>::EntryString(const bool& selected)
		{
			*MenuConsoleSizeStruct = NosStdLib::Console::GetConsoleSize(*MenuConsoleHandle, MenuConsoleScreenBI); /* Update values */

			std::wstring output = (selected ?
								   std::wstring(((MenuConsoleSizeStruct->Columns / 2) - EntryName.length() / 2) - 3, ' ') + L">> " + EntryName + std::wstring(4, ' ') + (*TypePointerStore ? L"[X]" : L"[ ]") + L" <<" :
								   std::wstring(((MenuConsoleSizeStruct->Columns / 2) - EntryName.length() / 2), ' ') + EntryName + std::wstring(4, ' ') + (*TypePointerStore ? L"[X]" : L"[ ]"));
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
				SetConsoleCursorPosition(*MenuConsoleHandle, {0, (SHORT)(inputStruct->CurrentIndex + inputStruct->TitleSize)});
				wprintf(EntryString(true).c_str());
				break;
			case EntryInputPassStruct::InputType::ArrowLeft:
				break;
			case EntryInputPassStruct::InputType::ArrowRight:
				break;
			}
		}

		/// <summary>
		/// Retuns the X values the entry string stop and start positions
		/// </summary>
		/// <returns>2 X positions in a int,int struct</returns>
		EntryStartAndLenght MenuEntry<bool>::EntryStartAndLenghtPosition()
		{
			*MenuConsoleSizeStruct = NosStdLib::Console::GetConsoleSize(*MenuConsoleHandle, MenuConsoleScreenBI); /* Update values */

			int x1 = ((MenuConsoleSizeStruct->Columns / 2) - (EntryName.length() / 2));

			return {x1, (int)(x1 + EntryName.length()+6)};
		}
	#pragma endregion

	#pragma region int
		/// <summary>
		/// create a wstring which shows the Entry name, value and is also centered
		/// </summary>
		/// <param name="selected">- if the entry is selected or not</param>
		/// <returns>wstring which shows the Entry name, value and is also centered</returns>
		std::wstring MenuEntry<int>::EntryString(const bool& selected)
		{
			*MenuConsoleSizeStruct = NosStdLib::Console::GetConsoleSize(*MenuConsoleHandle, MenuConsoleScreenBI); /* Update values */

			std::wstring output = (selected ?
								   std::wstring(((MenuConsoleSizeStruct->Columns / 2) - EntryName.length() / 2), ' ') + EntryName + std::wstring(4, ' ') + L"<" + std::to_wstring(*TypePointerStore) + L">" :
								   std::wstring(((MenuConsoleSizeStruct->Columns / 2) - EntryName.length() / 2), ' ') + EntryName + std::wstring(5, ' ') + std::to_wstring(*TypePointerStore));
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
				int ch;
				bool ContinueIntType = true;

				COORD NumberPosition = {(((MenuConsoleSizeStruct->Columns / 2) - EntryName.length() / 2) + EntryName.length() + 5), (inputStruct->CurrentIndex + inputStruct->TitleSize)};

				SetConsoleCursorPosition(*MenuConsoleHandle, NumberPosition);
				NosStdLib::Console::ShowCaret(true);

				while (ContinueIntType) /* TODO: Add message loop here */
				{
					ch = _getch();

					if (ch == NosStdLib::Definitions::ENTER)
					{
						ContinueIntType = false;
					}
					else if (ch == NosStdLib::Definitions::BACKSPACE)
					{
						// Coord for backspace cursor position editing
						COORD NewCoord;

						if (!NewInt.empty())
						{
							*MenuConsoleSizeStruct = NosStdLib::Console::GetConsoleSize(*MenuConsoleHandle, MenuConsoleScreenBI);

							NewCoord = {(SHORT)(MenuConsoleScreenBI->dwCursorPosition.X - 1), MenuConsoleScreenBI->dwCursorPosition.Y}; // create new coord with x-1 and same y
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
						{
							*TypePointerStore = INT_MIN;
						}
						else
						{
							*TypePointerStore = INT_MAX;
						}
					}
					catch (const std::exception& ex)
					{
						throw ex; /* if any other exception, do nothing */
					}
				}

				SetConsoleCursorPosition(*MenuConsoleHandle, {0, (SHORT)(inputStruct->CurrentIndex + inputStruct->TitleSize)});
				wprintf(EntryString(true).c_str());
				NosStdLib::Console::ShowCaret(false); /* hide the caret again */
				break;
			}
			case EntryInputPassStruct::InputType::ArrowLeft:
				(*TypePointerStore)--;
				SetConsoleCursorPosition(*MenuConsoleHandle, {0, (SHORT)(inputStruct->CurrentIndex + inputStruct->TitleSize)});
				wprintf(EntryString(true).c_str());
				break;
			case EntryInputPassStruct::InputType::ArrowRight:
				(*TypePointerStore)++;
				SetConsoleCursorPosition(*MenuConsoleHandle, {0, (SHORT)(inputStruct->CurrentIndex + inputStruct->TitleSize)});
				wprintf(EntryString(true).c_str());
				break;
			}
		}

		/// <summary>
		/// Retuns the X values the entry string stop and start positions
		/// </summary>
		/// <returns>2 X positions in a int,int struct</returns>
		EntryStartAndLenght MenuEntry<int>::EntryStartAndLenghtPosition()
		{
			*MenuConsoleSizeStruct = NosStdLib::Console::GetConsoleSize(*MenuConsoleHandle, MenuConsoleScreenBI); /* Update values */

			int x1 = ((MenuConsoleSizeStruct->Columns / 2) - (EntryName.length() / 2));

			return {x1, (int)(x1 + EntryName.length() + 5 + std::to_wstring(*TypePointerStore).length())};
		}
	#pragma endregion

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
								   std::wstring(((MenuConsoleSizeStruct->Columns / 2) - EntryName.length() / 2), ' ') + NosStdLib::RGB::NosRGB(212, 155, 55).MakeANSICode<wchar_t>() + EntryName);
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