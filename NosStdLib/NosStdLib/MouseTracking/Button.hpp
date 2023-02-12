#ifndef _BUTTON_NOSSTDLIB_HPP_
#define _BUTTON_NOSSTDLIB_HPP_

#include "../Functional.hpp"
#include "../Cast.hpp"
#include "../DynamicArray.hpp"
#include "../Vector.hpp"

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

		/// <summary>
		/// class which holds 2 points which are used for drawing a box
		/// </summary>
		class BoxSize
		{
		public:
			NosStdLib::Vector::VectorD2 PointOne;	/* Top Left Coord vector */
			NosStdLib::Vector::VectorD2 PointTwo;	/* Bottom Right Coord vector */
			NosStdLib::Vector::VectorD2 Offset;		/* Vector which holds offset values */

			BoxSize(){}

			/// <summary>
			/// Create object with int coords
			/// </summary>
			/// <param name="pointOneX">- Top X</param>
			/// <param name="pointOneY">- Left Y</param>
			/// <param name="pointTwoX">- Bottom X</param>
			/// <param name="pointTwoY">- Right Y</param>
			/// <param name="offsetX">(default = 0) - amount that gets added/taken away from X</param>
			/// <param name="offsetY">(default = 0) - amount that gets added/taken away from Y</param>
			BoxSize(const int& pointOneX, const int& pointOneY, const int& pointTwoX, const int& pointTwoY, const int& offsetX = 0, const int& offsetY = 0)
			{
				PointOne = NosStdLib::Vector::VectorD2(pointOneX, pointOneY);
				PointTwo = NosStdLib::Vector::VectorD2(pointTwoX, pointTwoY);
				Offset = NosStdLib::Vector::VectorD2(offsetX, offsetY);
			}

			/// <summary>
			/// Create object with Vector objects
			/// </summary>
			/// <param name="pointOne">- Top Left</param>
			/// <param name="pointTwo">- Bottom Right</param>
			/// <param name="Offset">- vector which containts amount that will get added/taken away from in calculations</param>
			BoxSize(const NosStdLib::Vector::VectorD2& pointOne, const NosStdLib::Vector::VectorD2& pointTwo, const NosStdLib::Vector::VectorD2& offset = NosStdLib::Vector::VectorD2(0,0))
			{
				PointOne = pointOne;
				PointTwo = pointTwo;
				Offset = offset;
			}
		};

		class Button
		{
		private:
			static inline NosStdLib::DynamicArray<Button*> ButtonArray;
			std::wstring ButtonText;
			BoxSize Position;

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
					if ((position.Y >= buttonPointer->Position.PointOne.Y && position.X >= buttonPointer->Position.PointOne.X)  && (position.Y <= buttonPointer->Position.PointOne.Y && position.X <= buttonPointer->Position.PointOne.X))
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

			Button(const std::wstring& buttonText, const BoxSize& position)
			{
				ButtonText = buttonText;
				Position = position;

				ButtonArray.Append(this);
			}

			~Button()
			{
				if (!(OnHover == nullptr)) { delete OnHover; OnHover = nullptr; }
				if (!(OnClick == nullptr)) { delete OnClick; OnClick = nullptr; }
				ButtonArray.ObjectRemove(this); /* remove self from array so the array doesn't call the delete operator again */
			}

			/* DOGSHIT CODE. REWRITE */


			std::wstring ModifyName(const std::wstring& nameInput, const int& size)
			{
				/* MAKE CHECKING FOR IF THE SIZE IS TOO SMALL FOR STRING */
				std::wstring outputString;

				if (nameInput.size() > size)
				{
					outputString = nameInput.substr(0, size - 2).append(L".."); /* IF STRING TOO LONG ERROR. LOOK HERE */
				}
				else{outputString = nameInput;}

				float spacingSize = size - outputString.size();
				outputString = (std::wstring(std::floorf(spacingSize / 2), L' ') + outputString + std::wstring(std::ceilf(spacingSize / 2), L' '));

				return outputString;
			}

			void PrintButton()
			{
				int middleSection = ((Position.PointTwo.X - Position.PointOne.X) - 1) / 2;

				std::wstring buttonString = (L'┌' + std::wstring((Position.PointTwo.Y - Position.PointOne.Y)-1, L'─') + L'┐');
				buttonString += L"\n";

				for (int i = 0; i < (Position.PointTwo.X - Position.PointOne.X)-1; i++)
				{
					buttonString += std::wstring(Position.left, L' ');
					buttonString += L"│";
					buttonString += (i == middleSection ? ModifyName(ButtonText, (Position.PointTwo.Y - Position.PointOne.Y)-1) : std::wstring((Position.PointTwo.Y - Position.PointOne.Y)-1, L' '));
					buttonString += L"│\n";
				}
				buttonString += std::wstring(Position.left, L' ');
				buttonString += (L'└' + std::wstring((Position.PointTwo.Y - Position.PointOne.Y)-1, L'─') + L'┘');
				buttonString += L"\n";

				SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), {(SHORT)Position.PointOne.X,(SHORT)Position.PointOne.Y} );
				wprintf(buttonString.c_str());
			}
		};
	}
}

#endif