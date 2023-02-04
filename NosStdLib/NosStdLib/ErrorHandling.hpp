#ifndef _ERRORHANDLING_NOSSTDLIB_HPP_
#define _ERRORHANDLING_NOSSTDLIB_HPP_

#include <Windows.h>
#include <string>

namespace NosStdLib
{
	namespace ErrorHandling
	{
	#pragma region GetLastErrorAsString
		std::wstring GetLastErrorAsString()
		{
			//Get the error message ID, if any.
			DWORD errorMessageID = ::GetLastError();
			if (errorMessageID == 0)
			{
				return std::wstring(); //No error message has been recorded
			}

			LPWSTR messageBuffer = nullptr;

			//Ask Win32 to give us the string version of that message ID.
			//The parameters we pass in, tell Win32 to create the buffer that holds the message for us (because we don't yet know how long the message string will be).
			size_t size = FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
										NULL, errorMessageID, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPWSTR)&messageBuffer, 0, NULL);

		   //Copy the error message into a std::string.
			std::wstring message(messageBuffer, size);

			//Free the Win32's string's buffer.
			LocalFree(messageBuffer);

			return message;
		}
	#pragma endregion
	}
}

#endif