#ifndef _RGB_NOSSTDLIB_HPP_
#define _RGB_NOSSTDLIB_HPP_

#include <string>
#include <cstdint>
#include <format>

namespace NosStdLib
{
	/// <summary>
	/// namespace which contains classes and functions for RGB
	/// </summary>
	namespace RGB
	{
		/// <summary>
		/// class to represent RGB
		/// </summary>
		class NosRGB
		{
		public:
			uint8_t R; /* Red */
			uint8_t G; /* Green */
			uint8_t B; /* Blue */

			NosRGB() {}

			NosRGB(uint8_t r, uint8_t g, uint8_t b)
			{
				R = r;
				G = g;
				B = b;
			}

			/// <summary>
			/// Create necesacry ANSI escape code to give wanted color
			/// </summary>
			/// <typeparam name="CharT">- string type</typeparam>
			/// <param name="value">- the RGB values wanted</param>
			/// <param name="foreGroundBackGround">(default = true) - if the ANSI should output foreground (true) or background (false)</param>
			/// <returns>the string containing the ANSI escape code</returns>
			template <typename CharT>
			std::basic_string<CharT> MakeANSICode(const bool& foreGroundBackGround = true) const
			{
				return std::vformat(NosStdLib::String::ConvertStringTypes<CharT, wchar_t>(foreGroundBackGround ? L"\033[38;2;{};{};{}m" : L"\033[48;2;{};{};{}m"), std::make_format_args<std::basic_format_context<std::back_insert_iterator<std::_Fmt_buffer<CharT>>, CharT>>(this->R, this->G, this->B));
			}
		};
	}

}

#endif