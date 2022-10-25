#ifndef _TEXTCOLOR_HPP_
#define _TEXTCOLOR_HPP_

#include "Global.hpp"
#include <codecvt>
#include <stdint.h>
#include <string>
#include <string_view>
#include <format>

namespace NosStdLib
{
	/// <summary>
	/// namespace contains items which are experimental (require more testing)
	/// </summary>
	namespace Experimental
	{
		/// <summary>
		/// function to convert global wstring to string
		/// </summary>
		/// <param name="globalWString">- the text to convert</param>
		/// <param name="output">- the output for overloads</param>
		/// <returns>pointer to output string</returns>
		std::string* ConvertGlobal(const std::wstring& globalWString, std::string* output)
		{
			*output = NosStdLib::Global::String::ToString(globalWString);
			return output;
		}

		/// <summary>
		/// function to convert global wstring to wstring
		/// </summary>
		/// <param name="globalWString">- the text to convert</param>
		/// <param name="output">- the output for overloads</param>
		/// <returns>pointer to output wstring</returns>
		std::wstring* ConvertGlobal(const std::wstring& globalWString, std::wstring* output)
		{
			*output = globalWString;
			return output;
		}
	}

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
			/* TODO: find or create terminoligy for a value that is constant in a function. */
			/* TODO: Create terminoligy table */
			
			std::basic_string<CharT> baseString;
			return std::vformat(*NosStdLib::Experimental::ConvertGlobal(L"\033[38;2;{};{};{}m", &baseString), std::make_format_args<std::basic_format_context<std::back_insert_iterator<std::_Fmt_buffer<CharT>>, CharT>>(values.R, values.G, values.B));
		}
	}
}
#endif