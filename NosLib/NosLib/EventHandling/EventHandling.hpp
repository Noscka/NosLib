#ifndef _EVENTHANDLING_NosLib_HPP_
#define _EVENTHANDLING_NosLib_HPP_

#include "../Functional.hpp"
#include "../DynamicArray.hpp"

#include <Windows.h>
#include <stdio.h>
#include <conio.h>

namespace NosLib
{
	/// <summary>
	/// Class which allows for storing functions for events
	/// </summary>
	class Event
	{
	private:
		NosLib::Functional::FunctionStoreBase* EventFunction = nullptr; /* pointer to FunctionStore which will be run on event trigger */
		bool AssignedFunction = false; /* If the EventFunction pointer was actually set */

		/* TODO: ALLOW FOR STACKING FUNCTIONS */
	public:
		/// <summary>
		/// Default constructor
		/// </summary>
		Event() {}

		/// <summary>
		/// constructor with specified function to run when event is triggered
		/// </summary>
		/// <param name="eventFunction">- Function Store object to run (use the none base version)</param>
		Event(NosLib::Functional::FunctionStoreBase* eventFunction)
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
		void AssignEventFunction(NosLib::Functional::FunctionStoreBase* eventFunction)
		{
			EventFunction = eventFunction;
			AssignedFunction = true;
		}

		/// <summary>
		/// Clears Event Function, so event does nothing
		/// </summary>
		/// <param name="deleteFunctionObject">(default = true) - if the functionstore object should get deleted on clear</param>
		void ClearEventFunction(const bool& deleteFunctionObject = true)
		{
			if (deleteFunctionObject)
			{
				delete EventFunction;
			}
			EventFunction = nullptr;
			AssignedFunction = false;
		}

		/// <summary>
		/// Runs the event function
		/// </summary>
		void TriggerEvent()
		{
			if (AssignedFunction && EventFunction != nullptr)
			{
				EventFunction->RunFunction();
			}
		#ifndef NDEBUG
			else
			{
				throw std::exception("Function wasn't set");
			}
		#endif
		}
	};

	/// <summary>
	/// Class which allows for storing specific functions for events
	/// </summary>
	template<class FuncType, typename ... VariadicArgs>
	class SpecializedEvent
	{
	private:
		NosLib::Functional::FunctionStore<FuncType, VariadicArgs...>* EventFunction = nullptr; /* pointer to FunctionStore which will be run on event trigger */
		bool AssignedFunction = false; /* If the EventFunction pointer was actually set */

		/* TODO: ALLOW FOR STACKING FUNCTIONS */
	public:
		/// <summary>
		/// Default constructor
		/// </summary>
		SpecializedEvent() {}

		/// <summary>
		/// constructor with specified function to run when event is triggered
		/// </summary>
		/// <param name="eventFunction">- Function Store object to run (use the none base version)</param>
		SpecializedEvent(NosLib::Functional::FunctionStore<FuncType, VariadicArgs...>* eventFunction)
		{
			AssignEventFunction(eventFunction);
		}

		/// <summary>
		/// constructor with specified function to run when event is triggered
		/// </summary>
		/// <param name="funcPointer">- function pointer</param>
		/// <param name="...args">- function arguments</param>
		SpecializedEvent(FuncType* funcPointer, VariadicArgs&& ... args)
		{
			AssignEventFunction(std::forward<FuncType*>(funcPointer), std::forward<VariadicArgs>(args)...);
		}

		~SpecializedEvent()
		{
			delete EventFunction;
		}

		/// <summary>
		/// Used to assigned the function to the event
		/// </summary>
		/// <param name="eventFunction">- Function Store object to run (use the none base version)</param>
		void AssignEventFunction(NosLib::Functional::FunctionStore<FuncType, VariadicArgs...>* eventFunction)
		{
			EventFunction = eventFunction;
			AssignedFunction = true;
		}

		/// <summary>
		/// Used to assigned the function to the event
		/// </summary>
		/// <param name="funcPointer">- function pointer</param>
		/// <param name="...args">- function arguments</param>
		void AssignEventFunction(FuncType* funcPointer, VariadicArgs&& ... args)
		{
			EventFunction = new NosLib::Functional::FunctionStore<FuncType, VariadicArgs...>(std::forward<FuncType*>(funcPointer), std::forward<VariadicArgs>(args)...);
			AssignedFunction = true;
		}

		/// <summary>
		/// Used to assigned the function to the event without preset arguements
		/// </summary>
		/// <param name="funcPointer">- function pointer</param>
		void AssignEventFunction(FuncType* funcPointer)
		{
			EventFunction = new NosLib::Functional::FunctionStore<FuncType, VariadicArgs...>(std::forward<FuncType*>(funcPointer));
			AssignedFunction = true;
		}

		/// <summary>
		/// Clears Event Function, so event does nothing
		/// </summary>
		/// <param name="deleteFunctionObject">(default = true) - if the functionstore object should get deleted on clear</param>
		void ClearEventFunction(const bool& deleteFunctionObject = true)
		{
			if (deleteFunctionObject)
			{
				delete EventFunction;
			}
			EventFunction = nullptr;
			AssignedFunction = false;
		}

		/// <summary>
		/// Runs the event function
		/// </summary>
		void TriggerEvent()
		{
			if (!EventFunction->HasPresetArguements()) /* if event function doesn't have preset variables, throw exception | TODO: convert to compile error */
			{
				throw std::logic_error("Cannot run functional without parameters if the arguments are not preset");
				return;
			}

			if (AssignedFunction && EventFunction != nullptr)
			{
				EventFunction->RunFunction();
			}
		#ifndef NDEBUG
			else
			{
				throw std::exception("Function wasn't set");
			}
		#endif
		}

		/// <summary>
		/// Runs the event function with custom arguments
		/// </summary>
		/// <param name="...args">- the custom arguments</param>
		void TriggerEvent(VariadicArgs& ... args)
		{
			if (AssignedFunction && EventFunction != nullptr)
			{
				EventFunction->RunFunction(std::forward<VariadicArgs>(args)...);
			}
		#ifndef NDEBUG
			else
			{
				throw std::exception("Function wasn't set");
			}
		#endif
		}

		/// <summary>
		/// Runs the event function with custom arguments
		/// </summary>
		/// <param name="...args">- the custom arguments</param>
		void TriggerEvent(VariadicArgs&& ... args)
		{
			if (AssignedFunction && EventFunction != nullptr)
			{
				EventFunction->RunFunction(std::forward<VariadicArgs>(args)...);
			}
		#ifndef NDEBUG
			else
			{
				throw std::exception("Function wasn't set");
			}
		#endif
		}
	};

	/// <summary>
	/// namespace which contains code for clean up once program gets shutdown
	/// </summary>
	namespace EventHandling
	{
        /// <summary>
        /// Privated namespace
        /// </summary>
        namespace
        {
            static NosLib::DynamicArray<void(*)()> ClosingCleanupFunctionArray; /* Array with list of functions to run when cleaning up */

            static inline bool Initialized = false;
        }

        BOOL WINAPI HandlerRoutine(DWORD eventCode)
        {
            switch (eventCode)
            {
            case CTRL_CLOSE_EVENT:
                for (int i = 0; i < ClosingCleanupFunctionArray.GetArrayIndexPointer(); i++)
                {
                    ClosingCleanupFunctionArray[i]();
                }
                return FALSE;
                break;
            }

            return TRUE;
        }

        /// <summary>
        /// Creates hook which handles events
        /// </summary>
        /// <returns>true for succesful and false for unsuccesful</returns>
        bool InitializeEventHandler()
        {
            Initialized = SetConsoleCtrlHandler(HandlerRoutine, TRUE);
            return Initialized;
        }

        /// <summary>
        /// Add function to cleanup array
        /// </summary>
        /// <param name="functionPointer">- pointer to function</param>
        void AddCleanupFunction(void(*functionPointer)())
        {
            if (!Initialized) /* if not Initialized, initialize so it works */
            {
                InitializeEventHandler();
            }

            ClosingCleanupFunctionArray.Append(functionPointer);
        }
	}
}

#endif