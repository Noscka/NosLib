#ifndef _DYNAMICMENUSYSTEM_NOSSTDLIB_HPP_
#define _DYNAMICMENUSYSTEM_NOSSTDLIB_HPP_

#include "String.hpp"
#include "DynamicArray.hpp"
#include "EventHandling.hpp"

namespace NosStdLib
{
	/* Subproject which is meant to handle Chats, so receiving and sending messages */
	namespace Chat
	{
		class DynamicChat
		{
		private:
			NosStdLib::DynamicArray<std::wstring> messages;

		public:
			Event* OnMessageSend = nullptr; /* pointer to event object which will trigger when user wants to send a message */

			DynamicChat()
			{

			}


			/// <summary>
			/// Starts the chat and a message loop
			/// </summary>
			void StartChat()
			{ /* TODO: Develop */
				while (true)
				{
					switch (MsgWaitForMultipleObjects(1, &eventHandle, FALSE, 5, QS_ALLINPUT))
					{

					}
				}
			}
		};
	}
}
#endif