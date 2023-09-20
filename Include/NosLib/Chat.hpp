#ifndef _DYNAMICCHAT_NosLib_HPP_
#define _DYNAMICCHAT_NosLib_HPP_

#include "Console.hpp"
#include "String.hpp"
#include "DynamicArray.hpp"
#include "EventHandling\EventHandling.hpp"
#include "Definitions.hpp"

#include <format>
#include <thread>
#include <queue>
#include <string>

namespace NosLib
{
	/* Subproject which is meant to handle Chats, so receiving and sending messages */
	namespace Chat
	{
		class DynamicChat
		{
		private:
			NosLib::DynamicArray<std::wstring> messages;

			HANDLE ConsoleHandle;									/* global Console Handle so it is synced across all operations and so it doesn't have to retrieved */
			CONSOLE_SCREEN_BUFFER_INFO ConsoleScreenBI;				/* global ConsoleScreenBI so it is synced across all operations */
			NosLib::Console::ConsoleSizeStruct ConsoleSizeStruct;/* a struct container for the Console columns and rows */

			HANDLE* ReceivedMessageEventHandle = nullptr;	/* Message Loop event which will triggered whenever a message was added using the "AddMessage" function */
			HANDLE* ReceivedUserInputEventHandle = nullptr; /* Message Loop event which will triggered whenever the user has input a string */
			HANDLE* InputOffsetChangedEventHandle = nullptr; /* Message Loop event which will triggered whenever the user changes input offset */

			bool ChatLoop = true; /* if the chat should continue looping (true -> yes, false -> no) */

			std::wstring CrossThread_UserString;	/* input for the message loop */

			bool AutoAddSentMessage = true;			/* if chat should automatically add "sent" message to self */
			std::wstring SelfAddSentMessageFormat;	/* the format to use for the self added sent message, needs one {} somewhere */
		public:
			SpecializedEvent<void(const std::wstring&), std::wstring> OnMessageSent; /* pointer to event object which will trigger when user wants to send a message */
			SpecializedEvent<void(const std::wstring&), std::wstring> OnMessageReceived; /* pointer to event object which will trigger when a message is added */

			/// <summary>
			/// Constructor
			/// </summary>
			/// <param name="autoAddSentMessage">(default = true) - if chat should automatically add "sent" message to self</param>
			/// <param name="selfAddSentMessageFormat">(default = L"{}") - the format to use for the self added sent message, needs one {} somewhere</param>
			DynamicChat(const bool& autoAddSentMessage = true, const std::wstring& selfAddSentMessageFormat = L"{}") /* TODO: MAKE CONSTANT AND ADD STATIC_ASSERT */
			{
				int count = NosLib::String::SubstringContainCount<wchar_t>(selfAddSentMessageFormat, L"{}");
				if (count == 0 || count > 1) /* if there is 0 or more then 1 bracket */
				{
					throw std::invalid_argument("Format string must contain only one set of {} brackets");
				}

				AutoAddSentMessage = autoAddSentMessage;
				SelfAddSentMessageFormat = selfAddSentMessageFormat;

				ConsoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
				ConsoleSizeStruct = NosLib::Console::GetConsoleSize(ConsoleHandle, &ConsoleScreenBI);
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

				OnMessageReceived.TriggerEvent(message.c_str());
			}

			/// <summary>
			/// if the chat loop is still going or not
			/// </summary>
			/// <returns>the chatloop bool</returns>
			bool GetChatLoopState()
			{
				return ChatLoop;
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

					if (ch == NosLib::Definitions::BACKSPACE)
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
						case NosLib::Definitions::ARROW_LEFT:
							if (NegativeOffset < CrossThread_UserString.size()) /* only if less then the size of the current string */
							{
								NegativeOffset++; /* increase backwards offset */
							}

							if (InputOffsetChangedEventHandle != nullptr)
							{
								SetEvent(*InputOffsetChangedEventHandle); /* tell message loop that there are keys in queue */
							}
							continue; /* Don't process character */

						case NosLib::Definitions::ARROW_RIGHT:
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

				NosLib::Console::SetConsoleBuffer();

				while (ChatLoop)
				{
					ConsoleSizeStruct = NosLib::Console::GetConsoleSize(ConsoleHandle, &ConsoleScreenBI); /* Every loop update the values */

					switch (WaitForMultipleObjects(3, handleArray, FALSE, INFINITE))
					{
					case WAIT_OBJECT_0 + 0: /* if event 0 (Received message) gets triggered */
					{
						SetConsoleCursorPosition(ConsoleHandle, {0,0});

						int iterateFrom = max((messages.GetLastArrayIndex() - (ConsoleSizeStruct.Rows - 1)), 0);

						for (int i = iterateFrom; i <= messages.GetLastArrayIndex() && i <= (ConsoleSizeStruct.Rows - 1) + iterateFrom; i++) /* for I, do rows-amount with min */
						{
							wprintf((messages[i] + std::wstring(ConsoleSizeStruct.Columns - messages[i].size(), L' ')).c_str());
						}

						/* NO BREAK on purpose */
					}
					case WAIT_OBJECT_0 + 1: /* if event 1 (User put in input) gets triggered */
					{
						SetConsoleCursorPosition(ConsoleHandle, {0, (SHORT)(ConsoleSizeStruct.Rows - 1)});

						wprintf((CrossThread_UserString + std::wstring(ConsoleSizeStruct.Columns - CrossThread_UserString.size(), L' ')).c_str());

						if (CrossThread_UserString.back() == Definitions::ENTER)
						{
							CrossThread_UserString.pop_back(); /* Remove last character since it is ENTER */

							if (AutoAddSentMessage) /* only add if enabled | TODO: convert to constexpr */
							{
								AddMessage(std::vformat(SelfAddSentMessageFormat, std::make_wformat_args(CrossThread_UserString)));
							}
							OnMessageSent.TriggerEvent(CrossThread_UserString);

							CrossThread_UserString.clear();

							NegativeOffset = 0;
						}

						/* NO BREAK on purpose */
					}
					case WAIT_OBJECT_0 + 2:
					{
						SetConsoleCursorPosition(ConsoleHandle, {(SHORT)(CrossThread_UserString.size() - NegativeOffset), (SHORT)(ConsoleSizeStruct.Rows - 1)});
						break;
					}

					case WAIT_TIMEOUT:
						break;
					}
				}
			}

		};
	}
}
#endif