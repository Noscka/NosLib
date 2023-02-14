#ifndef _MENUENTRY_NOSSTDLIB_HPP_
#define _MENUENTRY_NOSSTDLIB_HPP_

#include "../Definitions.hpp"
#include "../Console.hpp"
#include "../RGB.hpp"

#include <Windows.h>
#include <conio.h>

namespace NosStdLib
{
	namespace Menu
	{
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
			/// Update/set the variables such as pointers to ConsoleHandle, ConsoleScreenBI and ConsoleSizeStruct
			/// </summary>
			/// <returns></returns>
			void SetEntryVariables(HANDLE* menuConsoleHandle, CONSOLE_SCREEN_BUFFER_INFO* menuConsoleScreenBI, NosStdLib::Console::ConsoleSizeStruct* menuConsoleSizeStruct)
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
			MenuEntry(const std::wstring& name, EntryType* typePointerStore)
			{
				EntryName = name;
				TypePointerStore = typePointerStore;
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
		};

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
				SetConsoleCursorPosition(*MenuConsoleHandle, {0, (SHORT)(inputStruct->CurrentIndex + inputStruct->TitleSize)});
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
		std::wstring MenuEntry<int>::EntryString(const bool& selected)
		{
			*MenuConsoleSizeStruct = NosStdLib::Console::GetConsoleSize(*MenuConsoleHandle, MenuConsoleScreenBI); /* Update values */

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

				COORD NumberPosition = {(((MenuConsoleSizeStruct->Columns / 2) - EntryName.length() / 2) + EntryName.length() + 5), (inputStruct->CurrentIndex + inputStruct->TitleSize)};

				SetConsoleCursorPosition(*MenuConsoleHandle, NumberPosition);
				NosStdLib::Console::ShowCaret(true);

				while (ContinueIntType)
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
							*TypePointerStore = INT_MIN;
						else
							*TypePointerStore = INT_MAX;
					}
					catch (...)
					{
						/* if any other exception, do nothing */
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
	#pragma endregion

	#pragma region DynamicMenu
		/* Inside DynamicMenuSystem.hpp file since otherwise there was an issue of using an undefined class (which is the menu class) */
	#pragma endregion
	#pragma endregion
	}
}

#endif