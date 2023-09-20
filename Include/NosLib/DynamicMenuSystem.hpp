#ifndef _DYNAMICMENUSYSTEM_NosLib_HPP_
#define _DYNAMICMENUSYSTEM_NosLib_HPP_

#include "Definitions.hpp"
#include "UnicodeTextGenerator.hpp"
#include "Console.hpp"
#include "RGB.hpp"
#include "Clickable.hpp"
#include "Functional.hpp"
#include "DynamicArray/ArrayPositionTrack.hpp"
#include "MouseTracking/MouseTracking.hpp"

#include <Windows.h>
#include <conio.h>
#include <chrono>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <queue>

namespace NosLib
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
		/// The base of MenuEntry which only exists to allow for storing all the template types together
		/// </summary>
		class MenuEntryBase : public NosLib::ArrayPositionTrack::PositionTrack, public NosLib::Clickable::Clickable
		{
		protected:
			std::wstring EntryName; /* The entry name */

			DynamicMenu* ParentMenuPointer;									/* a pointer to the menu which parents/holds this entry */
			HANDLE* MenuConsoleHandle;										/* a pointer to the menu's MenuConsoleHandle, so its synced */
			CONSOLE_SCREEN_BUFFER_INFO* MenuConsoleScreenBI;				/* a pointer to the menu's MenuConsoleScreenBI, so its synced */
			NosLib::Console::ConsoleSizeStruct* MenuConsoleSizeStruct;	/* a pointer to the menu's MenuConsoleSizeStruct, so its synced */
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
			void SetEntryVariables(DynamicMenu* parentMenuPointer, HANDLE* menuConsoleHandle, CONSOLE_SCREEN_BUFFER_INFO* menuConsoleScreenBI, NosLib::Console::ConsoleSizeStruct* menuConsoleSizeStruct)
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
				*MenuConsoleSizeStruct = NosLib::Console::GetConsoleSize(*MenuConsoleHandle, MenuConsoleScreenBI); /* Update values */

				int SpaceLenght = NosLib::Cast<int>((MenuConsoleSizeStruct->Columns / 2) - EntryName.length() / 2);

				if constexpr (std::is_base_of_v<NosLib::Functional::FunctionStoreBase, EntryType>)
				{
					std::wstring output = (selected ?
										   std::wstring(SpaceLenght - 3, ' ') + L">> " + EntryName + L" <<" :
										   std::wstring(SpaceLenght - 0, ' ') + L"" + EntryName + L"");
					output += std::wstring(max(MenuConsoleSizeStruct->Columns - output.size(), 0), L' ') + L'\n';
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
				if constexpr (std::is_base_of_v<NosLib::Functional::FunctionStoreBase, EntryType>)
				{
					switch (inputStruct->inputType)
					{
					case EntryInputPassStruct::InputType::Enter:
						NosLib::Console::ClearScreen();
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
			/// Returns the X values the entry string stop and start positions
			/// </summary>
			/// <returns>2 X positions in a int,int struct</returns>
			EntryStartAndLenght EntryStartAndLenghtPosition()
			{
				*MenuConsoleSizeStruct = NosLib::Console::GetConsoleSize(*MenuConsoleHandle, MenuConsoleScreenBI); /* Update values */

				int x1 = NosLib::Cast<int>((MenuConsoleSizeStruct->Columns / 2) - (EntryName.length() / 2));

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
			NosLib::Console::ConsoleSizeStruct ConsoleSizeStruct;	/* a struct container for the Console columns and rows */
			NosLib::Console::ConsoleSizeStruct OldConsoleSizeStruct;/* a struct container for the old Console columns and rows */
			NosLib::DynamicArray<MenuEntryBase*> MenuEntryList;		/* array of MenuEntries */

			std::mutex InputThreadMutexControlling;
			std::condition_variable InputThreadConditionVariable;

			bool MenuLoop,				/* if the menu should continue looping (true -> yes, false -> no) */
				 GenerateUnicodeTitle,	/* if to generate a big Unicode title */
				 AddExitEntry,			/* if to add a quit option/entry at the bottom */
				 CenterTitle,			/* if the title should be centered */
				 AddedQuit,				/* if quit entry was already added. TODO: store int of position and if more entries are added (last isn't quit), move quit to last */
				 MenuFocused = false;	/* if the menu is focused (taking input and etc) */

			int TitleSize,				/* title size (for calculations where actual menu entries start) and also to create the clickable object boundaries */
				CurrentIndex,			/* Which item is currently selected */
				OldIndex;				/* Old index to know old index position */

			std::queue<int> CrossThread_CharQueue;	/* input queue for the message loop */
		public:
			/// <summary>
			/// DynamicMenu Constructor
			/// </summary>
			/// <param name="title">- title which will show at the top of menu</param>
			/// <param name="generateUnicodeTitle">(default = true) - if the title text should get converted into bubbled text</param>
			/// <param name="addExitEntry">(default = true) - if the menu should add in an exit entry at the bottom</param>
			/// <param name="centerTitle">(default = true) - if the menu should center the text</param>
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
				NosLib::MouseTracking::InitializeMouseTracking(); /* Initialize Mouse Tracking incase it was disabled */

				MenuLoop = true; /* incase menu was quit before */
				MenuFocused = true; /* set Menu focused to true */

				if (AddExitEntry && !AddedQuit)
				{
					AddMenuEntry(new MenuEntry(L"Quit", new NosLib::Functional::FunctionStore(&QuitMenu, this)));
					AddedQuit = true;
				}

				int ch, exCh; /* for getting input data */
				OldIndex = CurrentIndex = 0; /* Assign both old and current index ints */
				TitleSize = 0; /* title size (for calculations where actual menu entries start) */
				ConsoleSizeStruct = NosLib::Console::GetConsoleSize(ConsoleHandle, &ConsoleScreenBI); /* Update the ConsoleSize first time */
				OldConsoleSizeStruct = ConsoleSizeStruct;
				MSG msg;	/* MSG structer used for message loop */

				DrawMenu(); /* Draw menu first time */

				NosLib::Console::ShowCaret(false); /* Hide the caret */

				HANDLE eventHandle = CreateEvent(NULL, FALSE, FALSE, NULL);

				std::thread inputListenThread{[this, eventHandle]() {WaitForInput_Thread(eventHandle); }}; /* start waiting for input thread */

				while (MenuLoop)
				{
					switch (MsgWaitForMultipleObjects(1, &eventHandle, FALSE, 5, QS_ALLINPUT))
					{
					case WAIT_OBJECT_0 + 0: /* if event 0 (the input thread event) gets triggered */
						if (!CrossThread_CharQueue.empty())
						{
							ch = CrossThread_CharQueue.front();
							CrossThread_CharQueue.pop();
							if (!(ch && ch != 224))
							{
								exCh = CrossThread_CharQueue.front();
								CrossThread_CharQueue.pop();
							}

							if (ch == NosLib::Definitions::ENTER)
							{ /* WARNING: Might need to show the caret again not mattering what EntryType it is, as for some functions. it might be necessary */
								NosLib::MouseTracking::TemporaryTerminateMouseTracking();

								/* Notify thread that menu is out of focus */
								{
									std::lock_guard<std::mutex> lock(InputThreadMutexControlling);
								}
								MenuFocused = false; /* menu out of focus */
								InputThreadConditionVariable.notify_all();

								EntryInputPassStruct InputPassStruct{ CurrentIndex, TitleSize, this, EntryInputPassStruct::InputType::Enter, false };
								MenuEntryList[CurrentIndex]->EntryInput(&InputPassStruct);
								if (InputPassStruct.Redraw)
								{
									DrawMenu();
								}
								NosLib::Console::ShowCaret(false); /* hide the caret again */

								/* Notify thread that menu is back in focus */
								{
									std::lock_guard<std::mutex> lock(InputThreadMutexControlling);
								}
								MenuFocused = true; /* menu back in focus */
								InputThreadConditionVariable.notify_all();
								NosLib::MouseTracking::InitializeMouseTracking();
							}
							else if (!(ch && ch != 224))
							{
								switch (exCh)
								{
								case NosLib::Definitions::ARROW_UP:
									if (CurrentIndex > 0) /* Decrement only if larger the 0 */
									{
										CurrentIndex--; /* Decrement the Indentation */
									}
									break;
								case NosLib::Definitions::ARROW_DOWN:
									if (CurrentIndex < MenuEntryList.GetLastArrayIndex()) /* Increment only if smaller then List size */
									{
										CurrentIndex++; /* Increment the Indentation */
									}
									break;
								case NosLib::Definitions::ARROW_LEFT:
									{
										EntryInputPassStruct InputPassStruct{ CurrentIndex, TitleSize,this, EntryInputPassStruct::InputType::ArrowLeft, false };
										MenuEntryList[CurrentIndex]->EntryInput(&InputPassStruct);
										if (InputPassStruct.Redraw)
											DrawMenu();
										break;
									}
								case NosLib::Definitions::ARROW_RIGHT:
									{
										EntryInputPassStruct InputPassStruct{ CurrentIndex, TitleSize,this, EntryInputPassStruct::InputType::ArrowRight, false };
										MenuEntryList[CurrentIndex]->EntryInput(&InputPassStruct);
										if (InputPassStruct.Redraw)
											DrawMenu();
										break;
									}
								}
							}

							RedrawMenuCheck();

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

							if (CurrentIndex > OldIndex) /* Going Down */
							{
								SetConsoleCursorPosition(ConsoleHandle, {0, (SHORT)(TitleSize + CurrentIndex - 1)});
								wprintf((MenuEntryList[OldIndex]->EntryString(false) + MenuEntryList[CurrentIndex]->EntryString(true)).c_str());

								if ((TitleSize + CurrentIndex) + (ConsoleSizeStruct.Rows / 2) < 0)
								{
									finalPosition = {0,0};
								}
								else if ((TitleSize + CurrentIndex) + (ConsoleSizeStruct.Rows / 2) > MenuEntryList.GetLastArrayIndex()+1)
								{
									finalPosition = {0, (SHORT)(MenuEntryList.GetLastArrayIndex() + TitleSize)};
								}
								else
								{
									finalPosition = {0, (SHORT)((TitleSize + CurrentIndex) + (ConsoleSizeStruct.Rows / 2))};
								}
							}
							else if (CurrentIndex < OldIndex) /* Going Up */
							{
								SetConsoleCursorPosition(ConsoleHandle, {0, (SHORT)(TitleSize + CurrentIndex)});
								wprintf((MenuEntryList[CurrentIndex]->EntryString(true) + MenuEntryList[OldIndex]->EntryString(false)).c_str());

								if ((TitleSize + CurrentIndex) - (ConsoleSizeStruct.Rows / 2) <= 0)
								{
									finalPosition = {0,0};
								}
								else if ((TitleSize + CurrentIndex) - (ConsoleSizeStruct.Rows / 2) > MenuEntryList.GetLastArrayIndex()+1)
								{
									finalPosition = {0, (SHORT)(MenuEntryList.GetLastArrayIndex()-1)};
								}
								else
								{
									finalPosition = {0, (SHORT)((TitleSize + CurrentIndex) - (ConsoleSizeStruct.Rows / 2))};
								}
							}
							else
							{
								goto SkipSetConsoleCursorPosition;
							}

							SetConsoleCursorPosition(ConsoleHandle, finalPosition);

							SkipSetConsoleCursorPosition:

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

				MenuFocused = false;	/* menu out of focus, as it exits */
				NosLib::MouseTracking::TerminateMouseTracking(); /* Terminate mouse tracking hook */
				NosLib::Console::ClearScreen(); /* Clear the screen to remove the menu */
				NosLib::Console::ShowCaret(true); /* show the caret again */
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
					NosLib::MouseTracking::TemporaryTerminateMouseTracking();

					/* Notify thread that menu is out of focus */
					{
						std::lock_guard<std::mutex> lock((*parentMenu)->InputThreadMutexControlling);
					}
					(*parentMenu)->MenuFocused = false; /* menu out of focus */
					(*parentMenu)->InputThreadConditionVariable.notify_all();
					(*parentMenu)->CurrentIndex = *entryPosition;
					EntryInputPassStruct InputPassStruct{(*parentMenu)->CurrentIndex, (*parentMenu)->TitleSize,*parentMenu, EntryInputPassStruct::InputType::Enter, false};
					(*parentMenu)->MenuEntryList[(*parentMenu)->CurrentIndex]->EntryInput(&InputPassStruct);
					if (InputPassStruct.Redraw)
					{
						(*parentMenu)->DrawMenu();
					}
					NosLib::Console::ShowCaret(false); /* hide the caret again */

					/* Notify thread that menu is back in focus */
					{
						std::lock_guard<std::mutex> lock((*parentMenu)->InputThreadMutexControlling);
					}
					(*parentMenu)->MenuFocused = true; /* menu back in focus */
					(*parentMenu)->InputThreadConditionVariable.notify_all();
					NosLib::MouseTracking::InitializeMouseTracking();
					break;
				}

				case MouseEventEnum::OnEnterHover:
				{
					(*parentMenu)->CurrentIndex = *entryPosition;
					if ((*parentMenu)->OldIndex == (*parentMenu)->CurrentIndex) /* if old index and current index are equal */
					{
						return;
					}

					(*parentMenu)->RedrawMenuCheck();

					SetConsoleCursorPosition((*parentMenu)->ConsoleHandle, {0, (SHORT)((*parentMenu)->TitleSize + (*parentMenu)->OldIndex)});
					wprintf((*parentMenu)->MenuEntryList[(*parentMenu)->OldIndex]->EntryString(false).c_str());

					SetConsoleCursorPosition((*parentMenu)->ConsoleHandle, {0, (SHORT)((*parentMenu)->TitleSize + (*parentMenu)->CurrentIndex)});
					wprintf((*parentMenu)->MenuEntryList[(*parentMenu)->CurrentIndex]->EntryString(true).c_str());

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
				Entry->ModifyClickablePosition(NosLib::Dimension::DimensionD2<int16_t>(xxValue.X1, (TitleSize + MenuEntryList.GetItemCount()), xxValue.X2, (TitleSize + MenuEntryList.GetItemCount())));
				Entry->ModifyEnableBool(&MenuFocused);
				MenuEntryList.Append(Entry);
			}

			/// <summary>
			/// stops the menu MIGHT BE TEMPORARY
			/// </summary>
			void StopMenu()
			{
				MenuLoop = false;
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
					/* check if needs to be blocked */
					{
						std::unique_lock<std::mutex> lock(InputThreadMutexControlling);
						InputThreadConditionVariable.wait(lock, [this] { return MenuFocused; });
					}

					if (_kbhit()) /* check if there is any input to take in */
					{
						ch = _getch();
						CrossThread_CharQueue.push(ch);
						if (!(ch && ch != 224)) /* if the input WAS equal to 224, then wait for second input */
						{
							exCh = _getch();
							CrossThread_CharQueue.push(exCh);
						}
						SetEvent(eventHandle); /* tell message loop that there are keys in queue */
					}

					Sleep(10); /* cool down so it doesn't check 8905925157028157085 times per millisecond */
				}
				FlushConsoleInputBuffer(GetStdHandle(STD_INPUT_HANDLE));
				return;
			}

			/// <summary>
			/// Checks and redraws the menu if necessary
			/// </summary>
			void RedrawMenuCheck()
			{
				ConsoleSizeStruct = NosLib::Console::GetConsoleSize(ConsoleHandle, &ConsoleScreenBI);
				/* if the console dimensions haven't changed (console window has increased or decreased). then return */
				if (OldConsoleSizeStruct.Columns == ConsoleSizeStruct.Columns && OldConsoleSizeStruct.Rows == ConsoleSizeStruct.Rows)
				{
					return;
				}
				/* else redraw menu */

				OldConsoleSizeStruct = ConsoleSizeStruct;
				NosLib::Console::ShowCaret(false); /* hide the caret again */
				DrawMenu();
			}

			/// <summary>
			/// Draws the menu
			/// </summary>
			/// <param name="CurrentIndex">- currrent index</param>
			void DrawMenu()
			{
				//NosLib::MouseTracking::TemporaryTerminateMouseTracking();
				NosLib::Console::ClearScreen();

				ConsoleSizeStruct = NosLib::Console::GetConsoleSize(ConsoleHandle, &ConsoleScreenBI);

				std::wstring outputString; /* string for full "display" as it is the most performance efficient method */

				if (GenerateUnicodeTitle) /* If custom Title is true, its going to use the straight characters instead of generating a Unicode title */
				{
					outputString = NosLib::UnicodeTextGenerator::BasicUnicodeTextGenerate(ConsoleHandle, Title, CenterTitle); // add title with "ascii generator"
				}
				else
				{
					if (CenterTitle) {outputString = NosLib::String::CenterString<wchar_t>(Title);}
					else {outputString = Title;}
				}

				UpdateTitleSize();

				// for loop using counter to get the index so to add the >< to the selected option
				for (int i = 0; i <= MenuEntryList.GetLastArrayIndex(); i++)
				{
					if (i == CurrentIndex)
					{
						outputString += MenuEntryList[i]->EntryString(true);
					}
					else
					{
						outputString += MenuEntryList[i]->EntryString(false);
					}

					EntryStartAndLenght xxValue = MenuEntryList[i]->EntryStartAndLenghtPosition();
					MenuEntryList[i]->ModifyClickablePosition(NosLib::Dimension::DimensionD2<int16_t>(xxValue.X1, (TitleSize + i), xxValue.X2, (TitleSize + i)));
				}

				wprintf(outputString.c_str());

				SetConsoleCursorPosition(ConsoleHandle, { 0, (SHORT)(CurrentIndex) });
				//NosLib::MouseTracking::InitializeMouseTracking();
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
					TitleSize = NosLib::Cast<int>(std::count(Title.begin(), Title.end(), L'\n'));
					if (Title.back() != L'\n')
					{
						TitleSize++;
					}

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
			OnClick = new Event(new NosLib::Functional::FunctionStore(&DynamicMenu::MouseEventCallback, &ParentMenuPointer, this->GetArrayPositionPointer(), MouseEventEnum::OnClick));
			OnEnterHover = new Event(new NosLib::Functional::FunctionStore(&DynamicMenu::MouseEventCallback, &ParentMenuPointer, this->GetArrayPositionPointer(), MouseEventEnum::OnEnterHover));
			OnLeaveHover = new Event(new NosLib::Functional::FunctionStore(&DynamicMenu::MouseEventCallback, &ParentMenuPointer, this->GetArrayPositionPointer(), MouseEventEnum::OnLeaveHover));
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
			*MenuConsoleSizeStruct = NosLib::Console::GetConsoleSize(*MenuConsoleHandle, MenuConsoleScreenBI); /* Update values */

			std::wstring output = (selected ?
								   std::wstring(((MenuConsoleSizeStruct->Columns / 2) - EntryName.length() / 2) - 3, ' ') + L">> " + EntryName + std::wstring(4, ' ') + (*TypePointerStore ? L"[X]" : L"[ ]") + L" <<" :
								   std::wstring(((MenuConsoleSizeStruct->Columns / 2) - EntryName.length() / 2), ' ') + EntryName + std::wstring(4, ' ') + (*TypePointerStore ? L"[X]" : L"[ ]"));
			output += std::wstring(max(MenuConsoleSizeStruct->Columns - output.size(), 0), L' ') + L'\n';
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
		/// Returns the X values the entry string stop and start positions
		/// </summary>
		/// <returns>2 X positions in a int,int struct</returns>
		EntryStartAndLenght MenuEntry<bool>::EntryStartAndLenghtPosition()
		{
			*MenuConsoleSizeStruct = NosLib::Console::GetConsoleSize(*MenuConsoleHandle, MenuConsoleScreenBI); /* Update values */

			int x1 = NosLib::Cast<int>((MenuConsoleSizeStruct->Columns / 2) - (EntryName.length() / 2));

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
			*MenuConsoleSizeStruct = NosLib::Console::GetConsoleSize(*MenuConsoleHandle, MenuConsoleScreenBI); /* Update values */

			std::wstring output = (selected ?
								   std::wstring(((MenuConsoleSizeStruct->Columns / 2) - EntryName.length() / 2), ' ') + EntryName + std::wstring(4, ' ') + L"<" + std::to_wstring(*TypePointerStore) + L">" :
								   std::wstring(((MenuConsoleSizeStruct->Columns / 2) - EntryName.length() / 2), ' ') + EntryName + std::wstring(5, ' ') + std::to_wstring(*TypePointerStore));
			output += std::wstring(max(MenuConsoleSizeStruct->Columns - output.size(), 0), L' ') + L'\n';
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

				COORD NumberPosition = {NosLib::Cast<SHORT>(((MenuConsoleSizeStruct->Columns / 2) - EntryName.length() / 2) + EntryName.length() + 5), NosLib::Cast<SHORT>(inputStruct->CurrentIndex + inputStruct->TitleSize)};

				SetConsoleCursorPosition(*MenuConsoleHandle, NumberPosition);
				NosLib::Console::ShowCaret(true);

				while (ContinueIntType)
				{
					ch = _getch();

					if (ch == NosLib::Definitions::ENTER)
					{
						ContinueIntType = false;
					}
					else if (ch == NosLib::Definitions::BACKSPACE)
					{
						// Coord for backspace cursor position editing
						COORD NewCoord;

						if (!NewInt.empty())
						{
							*MenuConsoleSizeStruct = NosLib::Console::GetConsoleSize(*MenuConsoleHandle, MenuConsoleScreenBI);

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
					catch (const std::out_of_range&)
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
				NosLib::Console::ShowCaret(false); /* hide the caret again */
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
		/// Returns the X values the entry string stop and start positions
		/// </summary>
		/// <returns>2 X positions in a int,int struct</returns>
		EntryStartAndLenght MenuEntry<int>::EntryStartAndLenghtPosition()
		{
			*MenuConsoleSizeStruct = NosLib::Console::GetConsoleSize(*MenuConsoleHandle, MenuConsoleScreenBI); /* Update values */

			int x1 = NosLib::Cast<int>((MenuConsoleSizeStruct->Columns / 2) - (EntryName.length() / 2));

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
			*MenuConsoleSizeStruct = NosLib::Console::GetConsoleSize(*MenuConsoleHandle, MenuConsoleScreenBI); /* Update values */

			std::wstring output = (selected ?
								   std::wstring(((MenuConsoleSizeStruct->Columns / 2) - EntryName.length() / 2) - 3, ' ') + NosLib::RGB::NosRGB(212, 155, 55).MakeANSICode<wchar_t>() + L">> " + EntryName + L" <<" :
								   std::wstring(((MenuConsoleSizeStruct->Columns / 2) - EntryName.length() / 2), ' ') + NosLib::RGB::NosRGB(212, 155, 55).MakeANSICode<wchar_t>() + EntryName);
			output += std::wstring(max(MenuConsoleSizeStruct->Columns - output.size(), 0), L' ') + L"\033[0m\n";
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
				NosLib::Console::ClearScreen();
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