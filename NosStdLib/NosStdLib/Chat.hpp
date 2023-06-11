#ifndef _DYNAMICCHAT_NOSSTDLIB_HPP_
#define _DYNAMICCHAT_NOSSTDLIB_HPP_

#include "String.hpp"
#include "DynamicArray.hpp"
#include "EventHandling\EventHandling.hpp"
#include "Definitions.hpp"

#include <format>
#include <thread>
#include <queue>

namespace NosStdLib
{
	/* Subproject which is meant to handle Chats, so receiving and sending messages */
	namespace Chat
	{
		class DynamicChat
		{
		private:
			NosStdLib::DynamicArray<std::wstring> messages;

			HANDLE* ReceivedMessageEventHandle = nullptr;	/* Message Loop event which will triggered whenever a message was added using the "AddMessage" function */
			HANDLE* ReceivedUserInputEventHandle = nullptr; /* Message Loop event which will triggered whenever the user has input a string */

			bool ChatLoop; /* if the chat should continue looping (true -> yes, false -> no) */

			std::deque<int> CrossThread_CharQueue;	/* input queue for the message loop */
		public:
			Event* OnMessageSend = nullptr; /* pointer to event object which will trigger when user wants to send a message */
			Event* OnMessageReceived = nullptr; /* pointer to event object which will trigger when a message is added */

			DynamicChat()
			{

			}

			/// <summary>
			/// Used to add message to chat. will trigger an event to redraw the chat
			/// </summary>
			/// <param name="message">- the message to add</param>
			void AddMessage(const std::wstring& message)
			{
				messages.Append(message);
				
				if (ReceivedMessageEventHandle != nullptr)
				{
					SetEvent(*ReceivedMessageEventHandle); /* tell message loop that there are keys in queue */
				}
			}

		private:
			/// <summary>
			/// Function meant to run in a thread, will wait for user input and then send message to main loop
			/// </summary>
			/// <param name="eventHandle">- pointer to event handle</param>
			void TakeUserInput_Thread()
			{
				int ch;
				while (ChatLoop)
				{
					if (_kbhit()) /* check if there is any input to take in */
					{
						ch = _getch();
						CrossThread_CharQueue.push_back(ch);
						if (ReceivedUserInputEventHandle != nullptr)
						{
							SetEvent(*ReceivedUserInputEventHandle); /* tell message loop that there are keys in queue */
						}
					}

					Sleep(10); /* cool down so it doesn't check 8905925157028157085 times per millisecond */
				}
				return;
			}

		public:

			/// <summary>
			/// Starts the chat and a message loop
			/// </summary>
			void StartChat()
			{
				ChatLoop = true; /* incase chat was quit before */

				HANDLE tempReceivedMessageEventHandle = CreateEvent(NULL, FALSE, FALSE, NULL);		/* Triggered whenever a message was added using the "AddMessage" function */
				HANDLE tempReceivedUserInputEventHandle = CreateEvent(NULL, FALSE, FALSE, NULL);	/* Triggered whenever the user has input a string */

				HANDLE handleArray[2] = {tempReceivedMessageEventHandle, tempReceivedUserInputEventHandle};

				ReceivedMessageEventHandle = &handleArray[0];
				ReceivedUserInputEventHandle = &handleArray[1];

				std::thread messageReceiveThread([this]() {TakeUserInput_Thread(); });

				while (ChatLoop)
				{
					switch (WaitForMultipleObjects(2, handleArray, FALSE, INFINITE))
					{
					case WAIT_OBJECT_0 + 0: /* if event 0 (Received message) gets triggered */
						NosStdLib::Console::ClearScreen();

						wprintf(L"Received message, printing\n");

						for (std::wstring message : messages)
						{
							wprintf(std::format(L"{}\n", message).c_str());
						}
						break;
					case WAIT_OBJECT_0 + 1: /* if event 1 (User put in input) gets triggered */
						wprintf(std::format(L"{}", (wchar_t)CrossThread_CharQueue.back()).c_str());
						if (CrossThread_CharQueue.back() == Definitions::ENTER)
						{
							AddMessage(std::wstring(CrossThread_CharQueue.begin(), CrossThread_CharQueue.end()));
							CrossThread_CharQueue.empty();
						}
						break;
					case WAIT_TIMEOUT:
						break;
					}
				}
			}

		};
	}
}
#endif