#ifndef _RGB_NOSLIB_HPP_
#define _RGB_NOSLIB_HPP_

#include "String.hpp"

#include <string>
#include <cstdint>
#include <format>

namespace NosLib
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

			uint8_t IterateValue; /* how much iterate function should iterate by (85 for lego mode) */

			inline constexpr NosRGB() {}

			inline constexpr NosRGB(uint8_t r, uint8_t g, uint8_t b, uint8_t iterateValue = 1)
			{
				R = r;
				G = g;
				B = b;
				IterateValue = iterateValue;
			}

			/// <summary>
			/// Iterates the RGB values (for best effects, set either Red, Green or Blue to max which is 255)
			/// </summary>
			/// <returns>a reference to the object</returns>
			inline constexpr NosRGB& Iterate()
			{
				if (R > 0 && B == 0)
				{
					R -= IterateValue;
					G += IterateValue;
				}
				if (G > 0 && R == 0)
				{
					G -= IterateValue;
					B += IterateValue;
				}
				if (B > 0 && G == 0)
				{
					R += IterateValue;
					B -= IterateValue;
				}

				return *this;
			}

			/// <summary>
			/// Create necessary ANSI escape code to give wanted color
			/// </summary>
			/// <typeparam name="CharT">- string type</typeparam>
			/// <param name="value">- the RGB values wanted</param>
			/// <param name="foreGroundBackGround">(default = true) - if the ANSI should output foreground (true) or background (false)</param>
			/// <returns>the string containing the ANSI escape code</returns>
			template <typename CharT>
			inline constexpr std::basic_string<CharT> MakeANSICode(const bool& foreGroundBackGround = true) const
			{
				return std::vformat(NosLib::String::ConvertString<CharT, wchar_t>(foreGroundBackGround ? L"\033[38;2;{};{};{}m" : L"\033[48;2;{};{};{}m"), std::make_format_args<std::basic_format_context<std::back_insert_iterator<std::_Fmt_buffer<CharT>>, CharT>>(this->R, this->G, this->B));
			}

			template <typename CharT>
			static inline constexpr std::basic_string<CharT> ANSIResetCode()
			{
				return NosLib::String::ConvertString < CharT, wchar_t>(L"\033[0m\n");
			}

			/// <summary>
			/// Convert to COLORREF
			/// </summary>
			inline constexpr operator COLORREF() const
			{
				return RGB(R, G, B);
			}
		};
	}
}
#endif