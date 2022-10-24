#ifndef _TESTENVIROMENT_HPP_
#define _TESTENVIROMENT_HPP_

#include "DynamicArray.hpp"

namespace NosStdLib
{
	/// <summary>
	/// namespace which contains functions and classes which get used for testing (this namespace will most likely not have comments)
	/// </summary>
	namespace TestEnv
	{
		template<class CharT>
		bool IsNumberTest(const std::basic_string<CharT>& input)
		{
			/* Simple for loop, checking if any char isn't a digit */
			for (int i = 0; i < input.size(); i++)
			{
				if (!std::isdigit(input[i])) return false;
			}
			return true;
		}
	}
}

#endif