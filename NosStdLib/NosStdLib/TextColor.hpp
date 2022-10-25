#ifndef _TEXTCOLOR_HPP_
#define _TEXTCOLOR_HPP_

#include "Experimental.hpp"

#include <codecvt>
#include <stdint.h>
#include <string>
#include <string_view>
#include <format>

namespace NosStdLib
{
	/// <summary>
	/// This namespace contains items which are related to text coloring (mostly using ANSI)
	/// </summary>
	namespace TextColor
	{
		/// <summary>
		/// class to represent RGB
		/// </summary>
		class NosRGB
		{
		public:
			uint8_t R; /* Red */
			uint8_t B; /* Blue */
			uint8_t G; /* Green */

			NosRGB(){}

			NosRGB(uint8_t r, uint8_t b, uint8_t g)
			{
				R = r;
				B = b;
				G = g;
			}
		};

		/// <summary>
		/// Create necesacry ANSI escape code to give wanted color
		/// </summary>
		/// <typeparam name="CharT">- string type</typeparam>
		/// <param name="values">- the RGB values wanted</param>
		/// <returns>the string containing the ANSI escape code</returns>
		template <typename CharT>
		std::basic_string<CharT> MakeANSICode(const NosRGB& values)
		{
			/* TODO: Fix by making a function with overloads for each string type. (have a selected string type to be used for constants) */
			/* TODO: find or create terminoligy for a value that is constant in a function. */
			/* TODO: Create terminoligy table */
			
			std::basic_string<CharT> baseString;
			NosStdLib::Experimental::ConvertGlobal(L"\033[38;2;{};{};{}m", &baseString);
			return std::vformat(baseString, std::make_format_args<std::basic_format_context<std::back_insert_iterator<std::_Fmt_buffer<CharT>>, CharT>>(values.R, values.G, values.B));
		}
	}
}
#endif