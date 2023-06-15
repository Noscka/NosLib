#ifndef _DYNAMICCHAT_NOSSTDLIB_HPP_
#define _DYNAMICCHAT_NOSSTDLIB_HPP_

#include "Console.hpp"
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
			HANDLE* InputOffsetChangedEventHandle = nullptr; /* Message Loop event which will triggered whenever the user changes input offset */

			bool ChatLoop; /* if the chat should continue looping (true -> yes, false -> no) */

			std::wstring CrossThread_UserString;	/* input for the message loop */
		public:
			Event* OnMessageSent = nullptr; /* pointer to event object which will trigger when user wants to send a message */
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

				if (OnMessageReceived != nullptr)
				{
					OnMessageReceived->TriggerEvent();
				}
			}

		private:
			unsigned int NegativeOffset = 0;

			/// <summary>
			/// Function meant to run in a thread, will wait for user input and then send message to main loop
			/// </summary>
			/// <param name="eventHandle">- pointer to event handle</param>
			void TakeUserInput_Thread()
			{
				while (ChatLoop)
				{
					if (!_kbhit()) /* if no input, continue. done like this to reduce nesting */
					{
						Sleep(10); /* cool down so it doesn't check 8905925157028157085 times per millisecond */
						continue; /* skip execution */
					}

					int ch = _getch();

					if (ch == NosStdLib::Definitions::BACKSPACE)
					{
						if (!CrossThread_UserString.empty())
						{
							CrossThread_UserString.erase(CrossThread_UserString.end() - (NegativeOffset+1));
						}

						if (ReceivedUserInputEventHandle != nullptr)
						{
							SetEvent(*ReceivedUserInputEventHandle); /* tell message loop that there was an update to the string */
						}
						continue; /* skip printing and adding character to string */
					}
					else if (!(ch && ch != 224))
					{
						int exCh = _getch();

						switch (exCh)
						{
						case NosStdLib::Definitions::ARROW_LEFT:
							if (NegativeOffset < CrossThread_UserString.size()) /* only if less then the size of the current string */
							{
								NegativeOffset++; /* increase backwards offset */
							}

							if (InputOffsetChangedEventHandle != nullptr)
							{
								SetEvent(*InputOffsetChangedEventHandle); /* tell message loop that there are keys in queue */
							}
							continue; /* Don't process character */

						case NosStdLib::Definitions::ARROW_RIGHT:
							if (NegativeOffset > 0) /* only if offset is more then 0 */
							{
								NegativeOffset--;
							}

							if (InputOffsetChangedEventHandle != nullptr)
							{
								SetEvent(*InputOffsetChangedEventHandle); /* tell message loop that there are keys in queue */
							}
							continue; /* Don't process character */
						}
					}

					if (ch == Definitions::ENTER) /* ALWAYS put enter last so event handler below knows when to send the message */
					{
						CrossThread_UserString.push_back(ch);
					}
					else
					{
						CrossThread_UserString.insert(CrossThread_UserString.end() - NegativeOffset, ch);
					}

					if (ReceivedUserInputEventHandle != nullptr)
					{
						SetEvent(*ReceivedUserInputEventHandle); /* tell message loop that there are keys in queue */
					}
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
				HANDLE tempInputOffsetChangedEventHandle = CreateEvent(NULL, FALSE, FALSE, NULL);	/* Triggered whenever the user has input a string */

				HANDLE handleArray[3] = {tempReceivedMessageEventHandle, tempReceivedUserInputEventHandle, tempInputOffsetChangedEventHandle};

				ReceivedMessageEventHandle = &handleArray[0];
				ReceivedUserInputEventHandle = &handleArray[1];
				InputOffsetChangedEventHandle = &handleArray[2];

				std::thread messageReceiveThread([this]() {TakeUserInput_Thread(); });

				int lastCrossThreadStringSize = 0;

				while (ChatLoop)
				{
					switch (WaitForMultipleObjects(3, handleArray, FALSE, INFINITE))
					{
					case WAIT_OBJECT_0 + 0: /* if event 0 (Received message) gets triggered */
						NosStdLib::Console::ClearScreen();

						wprintf(L"Received message, printing\n");

						for (std::wstring message : messages)
						{
							wprintf(std::format(L"{}\n", message).c_str());
							wprintf(std::format(L"size: {}\n", message.size()).c_str());
						} /* NO BREAK on purpose */
					case WAIT_OBJECT_0 + 1: /* if event 1 (User put in input) gets triggered */
						SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), { 0, (SHORT)(NosStdLib::Console::GetConsoleSize().Rows - 1) });

						wprintf((CrossThread_UserString + (lastCrossThreadStringSize > CrossThread_UserString.size() ? L" " :  L"")).c_str());

						lastCrossThreadStringSize = CrossThread_UserString.size();
						if (CrossThread_UserString.back() == Definitions::ENTER)
						{
							CrossThread_UserString.pop_back(); /* Remove last character since it is ENTER */

							AddMessage(CrossThread_UserString);
							CrossThread_UserString.clear();

							if (OnMessageSent != nullptr)
							{
								OnMessageSent->TriggerEvent();
							}

							NegativeOffset = 0;
						} /* NO BREAK on purpose */
					case WAIT_OBJECT_0 + 2:
						SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), {(SHORT)(CrossThread_UserString.size()-NegativeOffset), (SHORT)(NosStdLib::Console::GetConsoleSize().Rows - 1)});
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