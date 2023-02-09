#ifndef _BUTTON_NOSSTDLIB_HPP_
#define _BUTTON_NOSSTDLIB_HPP_

#include "../Functional.hpp"
#include "../Cast.hpp"

namespace NosStdLib
{
	/// <summary>
	/// namespace which has items which are related to button creation and management.
	/// kinda temporary until I fully develop the idea and think of a better way to organize this
	/// </summary>
	namespace Button
	{
		class Event
		{
		private:
			NosStdLib::Functional::FunctionStoreBase* EventFunction;

		public:
			Event(NosStdLib::Functional::FunctionStoreBase* eventFunction)
			{
				EventFunction = eventFunction;
			}

			void TriggerEvent()
			{
				EventFunction->RunFunction();
			}
		};

		class Button
		{

		};
	}
}

#endif