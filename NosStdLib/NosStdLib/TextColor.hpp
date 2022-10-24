#ifndef _TEXTCOLOR_HPP_
#define _TEXTCOLOR_HPP_

#include <stdint.h>
#include <string>
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
		class RGB
		{
		public:
			uint8_t R; /* Red */
			uint8_t B; /* Blue */
			uint8_t G; /* Green */

			RGB(){}

			RGB(uint8_t r, uint8_t b, uint8_t g)
			{
				R = r;
				B = b;
				G = g;
			}
		};

		template <typename CharT>
		std::basic_string<CharT> MakeANSICode(const RGB& values)
		{
			return std::format<CharT>(std::basic_string < CharT>("\033[38;2;{};{};{}m", values.R, values.G, values.B));
		}
	}
}

#endif