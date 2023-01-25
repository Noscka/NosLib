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
	namespace CloseHandle
	{
        extern NosStdLib::DynamicArray<void(*)()> ClosingCleanupFunctionArray; /* Array with list of functions to run when cleaning up */

        BOOL WINAPI HandlerRoutine(DWORD eventCode)
        {
            switch (eventCode)
            {
            case CTRL_CLOSE_EVENT:
                for (int i = 0; i < ClosingCleanupFunctionArray.GetArrayIndexPointer(); i++)
                {
                    ClosingCleanupFunctionArray[i]();
                }
                return TRUE;
                break;
            }

            return TRUE;
        }

        /// <summary>
        /// Creates hook for what to do when shutting down
        /// </summary>
        /// <returns>true for succesful and false for unsuccesful</returns>
        bool InitializeCloseHandle()
        {
            return SetConsoleCtrlHandler(HandlerRoutine, TRUE);
        }
	}
}

#endif