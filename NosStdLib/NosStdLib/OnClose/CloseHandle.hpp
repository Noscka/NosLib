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
        NosStdLib::DynamicArray<void (*)(void)> ClosingCleanupFunctionArray; /* Array with list of functions to run when cleaning up */

        BOOL WINAPI HandlerRoutine(DWORD eventCode)
        {
            switch (eventCode)
            {
            case CTRL_CLOSE_EVENT:
                for (void* function : ClosingCleanupFunctionArray)
                {
                    (*function)();
                }
                return TRUE;
                break;
            }

            return TRUE;
        }

        //SetConsoleCtrlHandler(HandlerRoutine, TRUE);
        
	}
}

#endif