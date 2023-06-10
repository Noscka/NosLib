#ifndef _DYNAMICCHAT_NOSSTDLIB_HPP_
#define _DYNAMICCHAT_NOSSTDLIB_HPP_

#include "String.hpp"
#include "DynamicArray.hpp"
#include "EventHandling\EventHandling.hpp"

#include <format>
#include <thread>

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
			void TakeUserInput_Thread(HANDLE* eventHandle)
			{
				while (ChatLoop)
				{
					std::wstring output;
					std::getline(std::wcin, output);

					if (eventHandle != nullptr)
					{
						SetEvent(*eventHandle);
					}
				}
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
				ReceivedMessageEventHandle = &tempReceivedMessageEventHandle;
				ReceivedUserInputEventHandle = &tempReceivedUserInputEventHandle;

				std::thread messageReceiveThread([this, ReceivedUserInputEventHandle]() {TakeUserInput_Thread(ReceivedUserInputEventHandle); });

				MSG msg; /* MSG structer used for message loop */
				while (ChatLoop)
				{
					switch (MsgWaitForMultipleObjects(2, {ReceivedMessageEventHandle, ReceivedUserInputEventHandle}, FALSE, 5, QS_ALLINPUT))
					{
					case WAIT_OBJECT_0 + 0: /* if event 0 (Received message) gets triggered */
						NosStdLib::Console::ClearScreen();

						wprintf(L"Received message, printing\n");

						for (std::wstring message : messages)
						{
							wprintf(std::format(L"{}\n", message).c_str());
						}
					case WAIT_OBJECT_0 + 1: /* if event 1 (User put in input) gets triggered */
						break;
					case WAIT_OBJECT_0 + 2: /* if there is a message on the queue */
						PeekMessage(&msg, NULL, 0, 0, PM_REMOVE);
						TranslateMessage(&msg);
						DispatchMessage(&msg);
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