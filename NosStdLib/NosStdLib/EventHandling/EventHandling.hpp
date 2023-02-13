#ifndef _EVENTHANDLING_NOSSTDLIB_HPP_
#define _EVENTHANDLING_NOSSTDLIB_HPP_

#include "../Functional.hpp"
#include "../DynamicArray.hpp"

#include <Windows.h>
#include <stdio.h>
#include <conio.h>

namespace NosStdLib
{
	/// <summary>
	/// Class which allows for storing functions for events
	/// </summary>
	class Event
	{
	private:
		NosStdLib::Functional::FunctionStoreBase* EventFunction; /* pointer to FunctionStore which will be run on event trigger */

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
	/// namespace which contains code for clean up once program gets shutdown
	/// </summary>
	namespace EventHandling
	{
        /// <summary>
        /// Privated namespace
        /// </summary>
        namespace
        {
            static NosStdLib::DynamicArray<void(*)()> ClosingCleanupFunctionArray; /* Array with list of functions to run when cleaning up */

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