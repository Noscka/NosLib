#ifndef _DYNAMICCONTEXTMENU_NOSSTDLIB_HPP_
#define _DYNAMICCONTEXTMENU_NOSSTDLIB_HPP_

#include "Console.hpp"
#include "String.hpp"
#include "Vector.hpp"

#include <Windows.h>
#include <cstdint>
#include <string>

namespace NosStdLib
{
	namespace Menu
	{
		/// <summary>
		/// A context menu class which allows the creation of context menus (box with shows up on top of everything else)
		/// </summary>
		class ContextMenu
		{
		public:
			/// <summary>
			/// What the menu will ask for
			/// </summary>
			enum ContextType : uint8_t
			{
				Normal,
			};

		private:
			std::wstring Title;							/* Context menu title/basic description */
			std::wstring Description;					/* Description of what is needed (shown inside the box) */

			NosStdLib::Vector::VectorD2 MinSize;		/* Minimum size that is allowed for the box */
			NosStdLib::Vector::VectorD2 MaxSize;		/* Maximum size allowed for the box */
			NosStdLib::Vector::VectorD2 CurrentSize;	/* The current size of the box */
		public:
			/// <summary>
			/// A default constructor
			/// </summary>
			ContextMenu(){}

			/// <summary>
			/// Create context menu with title and description
			/// </summary>
			/// <param name="title">- Title shown on top</param>
			/// <param name="description">- Description shown inside the box</param>
			ContextMenu(const std::wstring& title, const std::wstring& description)
			{
				Title = title;
				Description = description;
			}

			/// <summary>
			/// Create context menu with title, description, minimum size and maximum size
			/// </summary>
			/// <param name="title">- Title shown on top</param>
			/// <param name="description">- Description shown inside the box</param>
			/// <param name="minSize">- Minimum size of the box (in characters) as VectorD2</param>
			/// <param name="maxSize">- Maximum size of the box (in characters) as VectorD2</param>
			ContextMenu(const std::wstring& title, const std::wstring& description, const NosStdLib::Vector::VectorD2& minSize, const NosStdLib::Vector::VectorD2 maxSize)
			{
				Title = title;
				Description = description;
				MinSize = minSize;
				MaxSize = maxSize;
			}

			std::wstring GenerateContextMenuString()
			{
				return L"";
			}
		};
	}
}

#endif