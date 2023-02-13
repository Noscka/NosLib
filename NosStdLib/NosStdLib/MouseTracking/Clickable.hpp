#ifndef _CLICKABLE_NOSSTDLIB_HPP_
#define _CLICKABLE_NOSSTDLIB_HPP_

#include "../Functional.hpp"
#include "../Cast.hpp"
#include "../DynamicArray.hpp"
#include "../DimentionVector.hpp"
#include "../EventHandling/EventHandling.hpp"


#include <math.h>
#include <cassert>

namespace NosStdLib
{
	/// <summary>
	/// namespace which has items which are related to button creation and management.
	/// </summary>
	namespace Clickable
	{
		class Button
		{
		private:
			static inline NosStdLib::DynamicArray<Button*> ButtonArray; /* Array containing all buttons */
			std::wstring ButtonText;									/* text that will be in the button */
			NosStdLib::Dimention::DimentionsD2 Position;				/* position of the button */

		public:
		#pragma region Static Functions
			/// <summary>
			/// Triggers Click event for all buttons at position
			/// </summary>
			/// <param name="position">- the position to trigger the event at</param>
			static void TriggerClickEventAtPosition(const NosStdLib::Vector::VectorD2<int16_t>& position)
			{
				for (Button* buttonPointer : ButtonArray)
				{
					if ((position.X >= buttonPointer->Position.PointOne.X && position.Y >= buttonPointer->Position.PointOne.Y) && (position.X <= buttonPointer->Position.PointTwo.X && position.Y <= buttonPointer->Position.PointTwo.Y))
					{
						buttonPointer->OnClick->TriggerEvent();
					}
				}
			}

			static bool CheckIfInside(const NosStdLib::Vector::VectorD2<int16_t>& position, const NosStdLib::Dimention::DimentionsD2& box)
			{
				return (position.X >= box.PointOne.X && position.Y >= box.PointOne.Y) &&
					   (position.X <= box.PointTwo.X && position.Y <= box.PointTwo.Y);
			}

			/// <summary>
			/// Triggers enter/leave Hover event for all buttons at position
			/// </summary>
			/// <param name="currentPosition">- current mouse position</param>
			/// <param name="lastPosition">- last mouse position</param>
			static void TriggerHoverEventAtPosition(const NosStdLib::Vector::VectorD2<int16_t>& currentPosition, const NosStdLib::Vector::VectorD2<int16_t>& lastPosition)
			{
				if(currentPosition == lastPosition){return;}

				for (Button* buttonPointer : ButtonArray)
				{
					if (CheckIfInside(currentPosition, buttonPointer->Position) == CheckIfInside(lastPosition, buttonPointer->Position)) /* if both are equal (both in or out), do nothing */
					{
						continue;
					}
					else if (CheckIfInside(currentPosition, buttonPointer->Position) && !CheckIfInside(lastPosition, buttonPointer->Position)) /* if current position is in and last wasn't mouse entered hover */
					{
						buttonPointer->OnEnterHover->TriggerEvent();
					}
					else if (!CheckIfInside(currentPosition, buttonPointer->Position) && CheckIfInside(lastPosition, buttonPointer->Position)) /* if current position isn't in and last was mouse left hover */
					{
						buttonPointer->OnLeaveHover->TriggerEvent();
					}
				}
			}

			/// <summary>
			/// gets all buttons in position into array
			/// </summary>
			/// <param name="position">- position to get buttons from</param>
			/// <returns>array of button pointers at position</returns>
			static NosStdLib::DynamicArray<Button*> GetButtonAtPosition(const NosStdLib::Vector::VectorD2<int16_t>& position)
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
			/// Prints all buttons to console (NEEDS TO BE LOOKED AT TO MAKE SURE THIS IS THE MOST EFFICIENT APPROACH)
			/// </summary>
			static void PrintAllButtons()
			{
				/* Either this or adding all the buttons together in a single string to be most efficient */
				for (Button* buttonPointer : ButtonArray)
				{
					buttonPointer->PrintButton(false);
				}
			}
		#pragma endregion

		//public:
			Event* OnEnterHover = nullptr; /* pointer to event object which will trigger when mouse enters hover over button */
			Event* OnLeaveHover = nullptr; /* pointer to event object which will trigger when mouse leaves hover over button */
			Event* OnClick = nullptr; /* pointer to event object which will tigger when mouse click on button */

			Button(){}

			static void DefaultHoverEventFunction(Button* object, const bool& inverse)
			{
				object->PrintButton(inverse);
			}

			/// <summary>
			/// Create button object
			/// </summary>
			/// <param name="buttonText">- text to show inside button</param>
			/// <param name="position">- position and dimentions of button</param>
			Button(const std::wstring& buttonText, const NosStdLib::Dimention::DimentionsD2& position)
			{
				ButtonText = buttonText;
				Position = position;

				OnEnterHover = new NosStdLib::Event(new NosStdLib::Functional::FunctionStore<void(NosStdLib::Clickable::Button*, const bool&), NosStdLib::Clickable::Button*, bool>(&DefaultHoverEventFunction, this, true));
				OnLeaveHover = new NosStdLib::Event(new NosStdLib::Functional::FunctionStore<void(NosStdLib::Clickable::Button*, const bool&), NosStdLib::Clickable::Button*, bool>(&DefaultHoverEventFunction, this, false));

				ButtonArray.Append(this);
			}

			~Button()
			{
				if (!(OnEnterHover == nullptr)) { delete OnEnterHover; OnEnterHover = nullptr; }
				if (!(OnLeaveHover == nullptr)) { delete OnLeaveHover; OnLeaveHover = nullptr; }
				if (!(OnClick == nullptr)) { delete OnClick; OnClick = nullptr; }
				ButtonArray.ObjectRemove(this); /* remove self from array so the array doesn't call the delete operator again */
			}

			/// <summary>
			/// Modify name to make sure it fits inside button
			/// </summary>
			/// <param name="nameInput">- input string</param>
			/// <param name="size">- size needed</param>
			/// <returns>modified name</returns>
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
			/// <param name="inverse">- if the colors should be inversed</param>
			/// /// <returns>generated button string</returns>
			std::wstring GenerateButtonString(const bool& inverse)
			{
				NosStdLib::Vector::VectorD2<int16_t> sizeVector = Position.CalculateSize(NosStdLib::Vector::VectorD2<int16_t>(-1, -1));
				std::wstring buttonString = L"";

				buttonString.reserve(((sizeVector.X + 1) * (sizeVector.Y + 1)) * (Position.PointOne.X * (sizeVector.Y + 1)));

				if (!inverse)
				{
					buttonString = (L'┌' + std::wstring(sizeVector.X, L'─') + L"┐\n");

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
				}
				else
				{
					buttonString = (L"\033[7m┌" + std::wstring(sizeVector.X, L'─') + L"┐\033[0m\n");

					int middleSectionPosition = sizeVector.Y / 2;
					for (int i = 0; i < sizeVector.Y; i++)
					{
						buttonString += std::wstring(Position.PointOne.X, L' ');
						buttonString += L"\033[7m│";
						buttonString += (i == middleSectionPosition ? ModifyName(ButtonText, sizeVector.X) : std::wstring(sizeVector.X, L' '));
						buttonString += L"│\033[0m\n";
					}
					buttonString += (std::wstring(Position.PointOne.X, L' ') + L"\033[7m");
					buttonString += (L'└' + std::wstring(sizeVector.X, L'─') + L"┘\033[0m\n");
				}

				return buttonString;
			}

			/// <summary>
			/// Print Button to console
			/// </summary>
			void PrintButton(const bool& inverse)
			{
				SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), Position.PointOne);
				wprintf(GenerateButtonString(inverse).c_str());
			}
		};
	}
}

#endif