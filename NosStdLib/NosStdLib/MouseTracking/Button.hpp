#ifndef _BUTTON_NOSSTDLIB_HPP_
#define _BUTTON_NOSSTDLIB_HPP_

#include "../Functional.hpp"
#include "../Cast.hpp"
#include "../DynamicArray.hpp"

#include <math.h>

namespace NosStdLib
{
	/// <summary>
	/// namespace which has items which are related to button creation and management.
	/// kinda temporary until I fully develop the idea and think of a better way to organize this
	/// </summary>
	namespace Button
	{
		/// <summary>
		/// Class which allows for storing functions for events
		/// </summary>
		class Event
		{
		private:
			NosStdLib::Functional::FunctionStoreBase* EventFunction; /* pointer to FunctionStore which will be run on event trigger */

		public:
			/// <summary>
			/// Default constructor
			/// </summary>
			Event(){}

			/// <summary>
			/// constructor with specified function to run when event is triggered
			/// </summary>
			/// <param name="eventFunction">- Function Store object to run (use the none base version)</param>
			Event(NosStdLib::Functional::FunctionStoreBase* eventFunction)
			{
				AssignEventFunction(eventFunction);
			}

			~Event()
			{
				delete EventFunction;
			}

			/// <summary>
			/// Used to assigned the function to the event
			/// </summary>
			/// <param name="eventFunction">- Function Store object to run (use the none base version)</param>
			void AssignEventFunction(NosStdLib::Functional::FunctionStoreBase* eventFunction)
			{
				EventFunction = eventFunction;
			}

			/// <summary>
			/// Runs the event function
			/// </summary>
			void TriggerEvent()
			{
				EventFunction->RunFunction();
			}

		};

		class Button
		{
		private:
			static inline NosStdLib::DynamicArray<Button*> ButtonArray;
			std::wstring ButtonText;
			RECT Position;

		public:
			/// <summary>
			/// Check if a button is in a position. if didn't find anything, returns nullptr
			/// </summary>
			/// <param name="position">- position to check</param>
			/// <returns>pointer to button (if one is found)</returns>
			static Button* CheckButtonAtPosition(COORD position)
			{
				for (Button* buttonPointer : ButtonArray)
				{
					if ((position.Y >= buttonPointer->Position.top && position.X >= buttonPointer->Position.left)  && (position.Y <= buttonPointer->Position.bottom && position.X <= buttonPointer->Position.right))
					{
						return buttonPointer;
					}
				}

				return nullptr;
			}

		//public:
			Event* OnHover = nullptr; /* pointer to event object which will trigger when mouse hovers over */
			Event* OnClick = nullptr; /* pointer to event object which will tigger when mouse click on button */

			Button(){}

			Button(const std::wstring& buttonText, const RECT& position)
			{
				ButtonText = buttonText;
				Position = position;

				ButtonArray.Append(this);
			}

			~Button()
			{
				if (!(OnHover == nullptr)) { delete OnHover; OnHover = nullptr; }
				if (!(OnClick == nullptr)) { delete OnClick; OnClick = nullptr; }
				ButtonArray.ObjectRemove(this); /* remove self from array so the array doesn't call the destroy on me */
			}

			/* DOGSHIT CODE. REWRITE */
			std::wstring ModifyName(const std::wstring& name)
			{
				int neededLenght = (Position.right - Position.left)-4;
				std::wstring output = name;
				if ((name.length()-4) > neededLenght)
				{
					output = (L" " + name.substr(0, neededLenght).append(L".. "));
				}
				else if ((name.length() - 4) < neededLenght)
				{
					float spacing = (neededLenght - (name.length() - 4));
					output = (std::wstring(std::floorf(spacing / 2), L' ') + name + std::wstring(std::ceilf(spacing / 2), L' '));
				}
				return output;
			}

			void PrintButton()
			{
				int middleSection = (Position.bottom - Position.top)/2;

				std::wstring buttonString = (L'┌' + std::wstring((Position.right - Position.left)-1, L'─') + L'┐');
				buttonString += L"\n";

				for (int i = 0; i < Position.bottom - Position.top; i++)
				{
					buttonString += std::wstring(Position.left, L' ');
					buttonString += L"│";
					buttonString += (i == middleSection ? ModifyName(ButtonText) : std::wstring((Position.right - Position.left)-1, L' '));
					buttonString += L"│\n";
				}
				buttonString += std::wstring(Position.left, L' ');
				buttonString += (L'└' + std::wstring((Position.right - Position.left)-1, L'─') + L'┘');
				buttonString += L"\n";

				SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), {(SHORT)Position.left, (SHORT)Position.top});
				wprintf(buttonString.c_str());
			}
		};
	}
}

#endif