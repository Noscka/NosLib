#ifndef _DYNAMICCONTEXTMENU_NOSSTDLIB_HPP_
#define _DYNAMICCONTEXTMENU_NOSSTDLIB_HPP_

#include "Global.hpp"
#include "String.hpp"

#include <Windows.h>
#include <cstdint>
#include <string>

namespace NosStdLib
{
	namespace Menu
	{
		class ContextMenu
		{
		public:
			enum ContextType : uint8_t
			{
				Normal,
			};

		private:
			std::wstring Title;
			std::wstring Description;
		public:
		};
	}
}

#endif