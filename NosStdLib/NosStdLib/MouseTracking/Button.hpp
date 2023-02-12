#ifndef _BUTTON_NOSSTDLIB_HPP_
#define _BUTTON_NOSSTDLIB_HPP_

#include "../Functional.hpp"
#include "../Cast.hpp"
#include "../DynamicArray.hpp"
#include "../Vector.hpp"

#include <math.h>
#include <cassert>

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
		class BoxDimentions
		{
		public:
			NosStdLib::Vector::VectorD2<int16_t> PointOne;	/* Top Left Coord vector */
			NosStdLib::Vector::VectorD2<int16_t> PointTwo;	/* Bottom Right Coord vector */
			NosStdLib::Vector::VectorD2<int16_t> Offset;	/* Vector which holds offset values */

			BoxDimentions(){}

			/// <summary>
			/// Create object with int coords
			/// </summary>
			/// <param name="pointOneX">- Top X</param>
			/// <param name="pointOneY">- Left Y</param>
			/// <param name="pointTwoX">- Bottom X</param>
			/// <param name="pointTwoY">- Right Y</param>
			/// <param name="offsetX">(default = 0) - amount that gets added/taken away from X</param>
			/// <param name="offsetY">(default = 0) - amount that gets added/taken away from Y</param>
			BoxDimentions(const int16_t& pointOneX, const int16_t& pointOneY, const int16_t& pointTwoX, const int16_t& pointTwoY, const int16_t& offsetX = 0, const int16_t& offsetY = 0)
			{
				PointOne = NosStdLib::Vector::VectorD2<int16_t>(pointOneX, pointOneY);
				PointTwo = NosStdLib::Vector::VectorD2<int16_t>(pointTwoX, pointTwoY);
				Offset = NosStdLib::Vector::VectorD2<int16_t>(offsetX, offsetY);
			}

			/// <summary>
			/// Create object with Vector objects
			/// </summary>
			/// <param name="pointOne">- Top Left</param>
			/// <param name="pointTwo">- Bottom Right</param>
			/// <param name="Offset">- vector which containts amount that will get added/taken away from in calculations</param>
			BoxDimentions(const NosStdLib::Vector::VectorD2<int16_t>& pointOne, const NosStdLib::Vector::VectorD2<int16_t>& pointTwo, const NosStdLib::Vector::VectorD2<int16_t>& offset = NosStdLib::Vector::VectorD2<int16_t>(0,0))
			{
				PointOne = pointOne;
				PointTwo = pointTwo;
				Offset = offset;
			}

			/// <summary>
			/// calculate size (pointeTwo-pointOne)
			/// </summary>
			/// <param name="offset">(default = true) - if calculation should take offset into considuration</param>
			/// <returns>VectorD2 with X and Y being sizes</returns>
			NosStdLib::Vector::VectorD2<int16_t> CalculateSize(const bool& offset = true)
			{
				if (offset)
				{
					return (PointTwo - PointOne) + Offset;
				}
				else
				{
					return (PointTwo - PointOne);
				}
			}

			/// <summary>
			/// calculate size with custom offset (pointeTwo-pointOne)
			/// </summary>
			/// <param name="offset">- the offset which will get used in the calculation</param>
			/// <returns>VectorD2 with X and Y being sizes that are offset</returns>
			NosStdLib::Vector::VectorD2<int16_t> CalculateSize(const NosStdLib::Vector::VectorD2<int16_t>& offset)
			{
				return (PointTwo - PointOne) + offset;
			}
		};

		class Button
		{
		private:
			static inline NosStdLib::DynamicArray<Button*> ButtonArray; /* Array containing all buttons */
			std::wstring ButtonText;	/* text that will be in the button */
			BoxDimentions Position;		/* position of the button */

		public:
			/// <summary>
			/// gets all buttons in position into array
			/// </summary>
			/// <param name="position">- position to check</param>
			/// <returns>array of button pointers</returns>
			static NosStdLib::DynamicArray<Button*> CheckButtonAtPosition(const NosStdLib::Vector::VectorD2<int16_t>& position)
			{
				NosStdLib::DynamicArray<Button*> returnButtonArray(5,2, false);

				for (Button* buttonPointer : ButtonArray)
				{
					if ((position.X >= buttonPointer->Position.PointOne.X && position.Y >= buttonPointer->Position.PointOne.Y)  && (position.X <= buttonPointer->Position.PointTwo.X && position.Y <= buttonPointer->Position.PointTwo.Y))
					{
						returnButtonArray.Append(buttonPointer);
					}
				}

				return returnButtonArray;
			}

			/// <summary>
			/// Check if a button is in a position. runs function passed in params
			/// </summary>
			/// <param name="position">- position to check</param>
			/// <param name="runFunc">- function to run when button found</param>
			static void CheckButtonAtPosition(const NosStdLib::Vector::VectorD2<int16_t>& position, void(*runFunc)(Button*))
			{
				for (Button* buttonPointer : ButtonArray)
				{
					if ((position.X >= buttonPointer->Position.PointOne.X && position.Y >= buttonPointer->Position.PointOne.Y) && (position.X <= buttonPointer->Position.PointTwo.X && position.Y <= buttonPointer->Position.PointTwo.Y))
					{
						(*runFunc)(buttonPointer);
					}
				}
			}

		//public:
			Event* OnEnterHover = nullptr; /* pointer to event object which will trigger when mouse enters hover over button */
			Event* OnLeaveHover = nullptr; /* pointer to event object which will trigger when mouse leaves hover over button */
			Event* OnClick = nullptr; /* pointer to event object which will tigger when mouse click on button */

			Button(){}

			Button(const std::wstring& buttonText, const BoxDimentions& position)
			{
				ButtonText = buttonText;
				Position = position;

				ButtonArray.Append(this);
			}

			~Button()
			{
				if (!(OnEnterHover == nullptr)) { delete OnEnterHover; OnEnterHover = nullptr; }
				if (!(OnLeaveHover == nullptr)) { delete OnLeaveHover; OnLeaveHover = nullptr; }
				if (!(OnClick == nullptr)) { delete OnClick; OnClick = nullptr; }
				ButtonArray.ObjectRemove(this); /* remove self from array so the array doesn't call the delete operator again */
			}

			/* DOGSHIT CODE. REWRITE */
			std::wstring ModifyName(const std::wstring& nameInput, const int& size)
			{
				if (size < 2){throw std::length_error("size in ModifyName function has to be 2 or more");}

				std::wstring outputString;

				if (nameInput.size() > size) /* if name is less then size, shorten it and add .. to the end to show it's been shortened */
				{
					outputString = nameInput.substr(0, size - 2).append(L"..");
				}
				else if (nameInput.size() < size) /* if name is more then size, center it in the space */
				{
					float spacingSize = size - nameInput.size();
					outputString = (std::wstring(std::floorf(spacingSize / 2), L' ') + nameInput + std::wstring(std::ceilf(spacingSize / 2), L' '));
				}
				else /* if name is same lenght as size. return name for efficiency */
				{
					return nameInput;
				}

				return outputString;
			}

			/// <summary>
			/// Generate string of the button
			/// </summary>
			/// <returns>generated button string</returns>
			std::wstring GenerateButtonString()
			{
				NosStdLib::Vector::VectorD2<int16_t> sizeVector = Position.CalculateSize(NosStdLib::Vector::VectorD2<int16_t>(-1, -1));

				std::wstring buttonString = (L'┌' + std::wstring(sizeVector.X, L'─') + L"┐\n");

				int middleSectionPosition = sizeVector.Y / 2;
				for (int i = 0; i < sizeVector.Y; i++)
				{
					buttonString += std::wstring(Position.PointOne.X, L' ');
					buttonString += L"│";
					buttonString += (i == middleSectionPosition ? ModifyName(ButtonText, sizeVector.X) : std::wstring(sizeVector.X, L' '));
					buttonString += L"│\n";
				}
				buttonString += std::wstring(Position.PointOne.X, L' ');
				buttonString += (L'└' + std::wstring(sizeVector.X, L'─') + L"┘\n");

				return buttonString;
			}

			/// <summary>
			/// Print Button to console
			/// </summary>
			void PrintButton()
			{
				SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), Position.PointOne);
				wprintf(GenerateButtonString().c_str());
			}
		};
	}
}

#endif