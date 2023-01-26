#ifndef _CLOSEHANDLE_NOSSTDLIB_HPP_
#define _CLOSEHANDLE_NOSSTDLIB_HPP_

#include "../DynamicArray.hpp"

#include <Windows.h>
#include <stdio.h>
#include <conio.h>

namespace NosStdLib
{
	/// <summary>
	/// namespace which contains code for clean up once program gets shutdown
	/// </summary>
	namespace EventHandling
	{
        static NosStdLib::DynamicArray<void(*)()> ClosingCleanupFunctionArray; /* Array with list of functions to run when cleaning up */

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
            return SetConsoleCtrlHandler(HandlerRoutine, TRUE);
        }
	}
}

#endif