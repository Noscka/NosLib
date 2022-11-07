#ifndef _TEXTCOLOR_NOSSTDLIB_HPP_
#define _TEXTCOLOR_NOSSTDLIB_HPP_

#include "Global.hpp"
#include "String.hpp"

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
	{}

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
			uint8_t G; /* Green */
			uint8_t B; /* Blue */

			NosRGB(){}

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
			std::basic_string<CharT> MakeANSICode(const bool& foreGroundBackGround = true)
			{
				return std::vformat(NosStdLib::String::ConvertStringTypes<wchar_t, CharT>(foreGroundBackGround ? L"\033[38;2;{};{};{}m" : L"\033[48;2;{};{};{}m"), std::make_format_args<std::basic_format_context<std::back_insert_iterator<std::_Fmt_buffer<CharT>>, CharT>>(this->R, this->G, this->B));
			}
		};

		/// <summary>
		/// Create necesacry ANSI escape code to give wanted color
		/// </summary>
		/// <typeparam name="CharT">- string type</typeparam>
		/// <param name="value">- the RGB values wanted</param>
		/// <param name="foreGroundBackGround">(default = true) - if the ANSI should output foreground (true) or background (false)</param>
		/// <returns>the string containing the ANSI escape code</returns>
		template <typename CharT>
		std::basic_string<CharT> MakeANSICode(const NosStdLib::TextColor::NosRGB& value, const bool& foreGroundBackGround = true)
		{
			/* TODO: make this function use the above function */

			//return value.MakeANSICode<CharT>(foreGroundBackGround);
			return std::vformat(NosStdLib::String::ConvertStringTypes<wchar_t, CharT>(foreGroundBackGround ? L"\033[38;2;{};{};{}m" : L"\033[48;2;{};{};{}m"), std::make_format_args<std::basic_format_context<std::back_insert_iterator<std::_Fmt_buffer<CharT>>, CharT>>(value.R, value.G, value.B));
		}

		/// <summary>
		/// namespace which contains functions which aren't useful but are entraiting or fun
		/// </summary>
		namespace Fun
		{
			/// <summary>
			/// Function which creates a line of characters with the specified color
			/// </summary>
			/// <param name="columnCount">- the amount of columns</param>
			/// <param name="rgbValue">(default = (20, 180, 170)) - the RGB value wanted</param>
			/// <returns>a row with a color</returns>
			std::wstring ColorRow(const int& columnCount, const NosStdLib::TextColor::NosRGB& rgbValue = NosStdLib::TextColor::NosRGB(20, 180, 170))
			{
				return  NosStdLib::TextColor::MakeANSICode<wchar_t>(rgbValue, true) + std::wstring(max(columnCount, 0), L'█') + L"\033[0m";
			}

			/// <summary>
			/// function which iterates the ColorRow function to make the console output nice colors
			/// </summary>
			/// <param name="singleRow">(singleRow = false) - if it should only output on 1 row or file the whole console</param>
			/// <param name="sleepSpeed">(default = 1) - the speed to output at</param>
			void IterateRainbow(const bool& singleRow = false, const int& sleepSpeed = 1)
			{
				NosStdLib::Global::Console::ShowCaret(false);

				HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
				CONSOLE_SCREEN_BUFFER_INFO csbi;
				GetConsoleScreenBufferInfo(consoleHandle, &csbi);

				uint8_t R = 255, G = 0, B = 0;
				uint8_t changeValue = 1; /* 85 for lego mode */

				while (true)
				{
					if (R > 0 && B == 0)
					{
						R -= changeValue;
						G += changeValue;
					}
					if (G > 0 && R == 0)
					{
						G -= changeValue;
						B += changeValue;
					}
					if (B > 0 && G == 0)
					{
						R += changeValue;
						B -= changeValue;
					}

					wprintf(ColorRow(csbi.srWindow.Right - csbi.srWindow.Left + 1, NosStdLib::TextColor::NosRGB(R, G, B)).c_str());
					if(singleRow)
						SetConsoleCursorPosition(consoleHandle, { 0, 0 });
					Sleep(sleepSpeed);
				}

				NosStdLib::Global::Console::ShowCaret(true);
			}
		}
	}
}
#endif