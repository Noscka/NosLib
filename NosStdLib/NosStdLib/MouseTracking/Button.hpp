#ifndef _BUTTON_NOSSTDLIB_HPP_
#define _BUTTON_NOSSTDLIB_HPP_

#include "../Functional.hpp"
#include "../Cast.hpp"
#include "../DynamicArray.hpp"

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
		};
	}
}

#endif