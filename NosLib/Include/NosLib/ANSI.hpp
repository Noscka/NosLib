#ifndef _ANSI_NOSLIB_HPP_
#define _ANSI_NOSLIB_HPP_

#include <NosLib/RGB.hpp>

#include <string>
#include <format>

namespace NosLib
{
	inline std::string CreateANSIColorCode(const NosLib::RGB& value)
	{
		return std::format("\033[38;2;{};{};{}m", value.R, value.G, value.B);
	}

	inline std::string CreateANSIBackgroundColorCode(const NosLib::RGB& value)
	{
		return std::format("\033[48;2;{};{};{}m", value.R, value.G, value.B);
	}

	constexpr inline  std::string CreateANSIResetCode()
	{
		return "\033[0m";
	}

	inline std::string ColorString(const std::string& string, const NosLib::RGB& value)
	{
		return CreateANSIColorCode(value) + string + CreateANSIResetCode();
	}
}

#endif // _ANSI_NOSLIB_HPP_