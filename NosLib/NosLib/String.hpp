#ifndef _STRING_NosLib_HPP_
#define _STRING_NosLib_HPP_

#include "DynamicArray.hpp"
#include "TypeTraits.hpp"
#include "Cast.hpp"

#include <Windows.h>
#include <stringapiset.h>
#include <string>
#include <sstream>
#include <vector>
#include <minmax.h>
#include <algorithm>
#include <iterator>
#include <set>

namespace NosLib
{
	/// <summary>
	/// namespace for items which are related to strings (both string and wstring)
	/// </summary>
	namespace String
	{
	#pragma region String Conversion
		/// <summary>
		/// Converts any string type to any other string type. template <ToType, FromType>
		/// </summary>
		/// <typeparam name="StringTo">- the string output type</typeparam>
		/// <typeparam name="StringFrom">- the string input type</typeparam>
		/// <param name="strIn">- string input</param>
		/// <returns>converted string</returns>
		template<typename StringTo, typename StringFrom>
		std::basic_string<StringTo> ConvertString(const std::basic_string<StringFrom>& strIn)
		{
			if constexpr (std::is_same_v<StringTo, StringFrom>) /* if same character type, then just return string */
			{
				return strIn;
			}
			else /* else, do standard conversion*/
			{
				std::basic_string<StringTo> strOut;
				std::copy(strIn.begin(), strIn.end(), std::back_insert_iterator(strOut));
				return strOut;
			}
		}

		const auto ToString = ConvertString<char, wchar_t>;
		const auto ToWstring = ConvertString<wchar_t, char>;
	#pragma endregion

	#pragma region Character Conversion
		/// <summary>
		/// Converts single characters using casting
		/// </summary>
		/// <typeparam name="CharTo">- Character output type</typeparam>
		/// <typeparam name="CharFrom">- Character input type</typeparam>
		/// <param name="charIn">- character input</param>
		/// <returns>converted character</returns>
		template<typename CharTo, typename CharFrom>
		CharTo ConvertCharacter(const CharFrom& charIn)
		{
			static_assert(NosLib::TypeTraits::is_character<CharTo>::value, "type getting converted into must be character type");
			static_assert(NosLib::TypeTraits::is_character<CharFrom>::value, "type getting converted from must be character type");

			return NosLib::Cast::Cast<CharTo, CharFrom>(charIn);
		}
	#pragma endregion

	#pragma region IsNumber
		/// <summary>
		/// Check if string is number (with or without signs)
		/// </summary>
		/// <typeparam name="CharT">- string type</typeparam>
		/// <param name="str">- string to check</param>
		/// <param name="allowSigns">(default = true) - if it should allow signs at the start of the string (-123 or +123)</param>
		/// <returns>if string is valid number</returns>
		template <typename CharT>
		bool IsNumber(const std::basic_string<CharT>& str, const bool& allowSigns = true)
		{
			/* Iterator int, allows for changing start position */
			int Iteration = 0;
			if (allowSigns)
			{
				/* if allowSigns is true, check if first character is either - or + or a number */
				if ((str[0] != NosLib::String::ConvertCharacter<CharT, wchar_t>(L'-') && str[0] != NosLib::String::ConvertCharacter<CharT, wchar_t>(L'+')) && !std::isdigit(str[0]))
					return false;

				Iteration = 1; /* Make Iterator go up 1 so for loop doesn't check first character */
			}

			/* Simple for loop, checking if any char isn't a digit */
			for (; Iteration < str.size(); Iteration++)
			{
				if (!std::isdigit(str[Iteration])) return false;
			}

			return true;
		}
	#pragma endregion

	#pragma region Split
		/// <summary>
		/// Split a string into a vectory array using a delimiter
		/// </summary>
		/// <typeparam name="CharT">- string type</typeparam>
		/// <param name="result">- the vector that will get modified</param>
		/// <param name="input">- the input that will get split</param>
		/// <param name="delimiter">(default = L' ') - delimiter which will determine the split</param>
		/// <returns>pointer to modified vector</returns>
		template <typename CharT>
		std::vector<std::basic_string<CharT>>* Split(std::vector<std::basic_string<CharT>>* result, const std::basic_string<CharT>& input, const CharT& delimiter = L' ')
		{
			std::basic_string<CharT> tmp;
			std::basic_stringstream<CharT> ss(input);

			while (std::getline(ss, tmp, delimiter))
			{
				result->push_back(tmp);
			}

			return result;
		}

		/// <summary>
		/// Split a string into a DynamicArray Entries using a delimiter
		/// </summary>
		/// <typeparam name="CharT">- string type</typeparam>
		/// <param name="result">- the DynamicArray that will get modified</param>
		/// <param name="input">- the input that will get split</param>
		/// <param name="delimiter">(default = L' ') - delimiter which will determine the split</param>
		/// <returns>pointer to modified DynamicArray</returns>
		template <typename CharT>
		NosLib::DynamicArray<std::basic_string<CharT>>* Split(NosLib::DynamicArray<std::basic_string<CharT>>* result, const std::basic_string<CharT>& input, const CharT& delimiter = L' ')
		{
			std::basic_string<CharT> tmp;
			std::basic_stringstream<CharT> ss(input);

			while (std::getline(ss, tmp, delimiter))
			{
				result->Append(tmp);
			}

			return result;
		}
	#pragma endregion

	#pragma region CenterString
		/// <summary>
		/// Center string with custom console Handle
		/// </summary>
		/// <typeparam name="CharT">- string type</typeparam>
		/// <param name="consoleHandle">- Custom Console Handle</param>
		/// <param name="input">- string to center</param>
		/// <param name="all">(default = true) - if it should center just first line or all lines</param>
		/// <returns>centered string</returns>
		template <typename CharT>
		std::basic_string<CharT> CenterString(const HANDLE& consoleHandle, const std::basic_string<CharT>& input, const bool& rightPadding = false, const bool& all = true)
		{
			CONSOLE_SCREEN_BUFFER_INFO csbi; /* TODO: change from using consoleHandle or add overload to use ConsoleSizeStruct */
			GetConsoleScreenBufferInfo(consoleHandle, &csbi);
			int columns = csbi.srWindow.Right - csbi.srWindow.Left + 1;
			if (all)
			{
				NosLib::DynamicArray<std::basic_string<CharT>> inputSplit;
				std::basic_string<CharT> output;
				NosLib::String::Split<CharT>(&inputSplit, input, L'\n');

				for (std::basic_string<CharT> Singleinput : inputSplit)
				{
					output += (std::basic_string<CharT>(((columns / 2) - Singleinput.size() / 2), ' ') + Singleinput + (rightPadding ? std::wstring(max((columns - (Singleinput.size() + ((columns / 2) - Singleinput.size() / 2))), 0), L' ') : L"") + NosLib::String::ConvertCharacter<CharT, wchar_t>(L'\n'));
				}

				return output;
			}
			else
			{
				return (std::basic_string<CharT>(((columns / 2) - input.size() / 2), ' ') + input + (rightPadding ? std::wstring(max((columns - (input.size() + ((columns / 2) - input.size() / 2))), 0), L' ') : L"") + NosLib::String::ConvertCharacter<CharT, wchar_t>(L'\n'));
			}
		}

		/// <summary>
		/// Center rvalue string with custom console Handle
		/// </summary>
		/// <typeparam name="CharT">- string type</typeparam>
		/// <param name="consoleHandle">- Custom Console Handle</param>
		/// <param name="input">- rvalue string to center</param>
		/// <param name="all">(default = true) - if it should center just first line or all lines</param>
		/// <returns>centered string</returns>
		template <typename CharT>
		std::basic_string<CharT> CenterString(const HANDLE& consoleHandle, const std::basic_string<CharT>&& input, const bool& rightPadding = false, const bool& all = true)
		{
			return CenterString<CharT>(consoleHandle, input, rightPadding, all);
		}

		/// <summary>
		/// Center string 
		/// </summary>
		/// <typeparam name="CharT">- string type</typeparam>
		/// <param name="input">- string to center</param>
		/// <param name="all">(default = true) - if it should center just first line or all lines</param>
		/// <returns>centered string</returns>
		template <typename CharT>
		std::basic_string<CharT> CenterString(const std::basic_string<CharT>& input, const bool& rightPadding = false, const bool& all = true)
		{
			return CenterString<CharT>(GetStdHandle(STD_OUTPUT_HANDLE), input, rightPadding, all);
		}

		/// <summary>
		/// Center rvalue string
		/// </summary>
		/// <typeparam name="CharT">- string type</typeparam>
		/// <param name="input">- rvalue string to center</param>
		/// <param name="all">(default = true) - if it should center just first line or all lines</param>
		/// <returns>centered string</returns>
		template <typename CharT>
		std::basic_string<CharT> CenterString(const std::basic_string<CharT>&& input, const bool& rightPadding = false, const bool& all = true)
		{
			return CenterString<CharT>(GetStdHandle(STD_OUTPUT_HANDLE), input, rightPadding, all);
		}
	#pragma endregion

	#pragma region FindNextWord
		/// <summary>
		/// finds next word from position
		/// </summary>
		/// <typeparam name="CharT">- string type</typeparam>
		/// <param name="string">- the string to look through</param>
		/// <param name="startPosition">- position to start looking from</param>
		/// <param name="word">- pointer to string which will get modified to the word found</param>
		/// <param name="wordStartPosition">- pointer to int which will get modified with start position of the word</param>
		/// <param name="delimiters">(default = L' ') - what character to ignore</param>
		/// <returns>the word</returns>
		template <typename CharT>
		std::basic_string<CharT> FindNextWord(const std::basic_string<CharT>& string, const int& startPosition, std::basic_string<CharT>* word, int* wordStartPosition, const CharT& delimiter = L' ')
		{
			uint16_t wordRange = 0, wordStartPos = 0;
			bool wordCounting = false;
			for (int i = startPosition; i < string.length(); i++)
			{
				if (string[i] != delimiter && !wordCounting)
				{
					wordCounting = true;
					wordStartPos = i;
					*wordStartPosition = i;
					wordRange++;
				}

				if (wordCounting && string[i] != delimiter)
				{
					wordRange++;
				}

				if (wordCounting && string[i] == delimiter)
				{
					break;
				}
			}

			if (wordRange != 0)
			{
				*word = string.substr(wordStartPos, wordRange);
			}

			return *word;
		}


		/// <summary>
		/// finds next word from position without pointer returns
		/// </summary>
		/// <typeparam name="CharT">- string type</typeparam>
		/// <param name="string">- the string to look through</param>
		/// <param name="startPosition">- position to start looking from</param>
		/// <param name="delimiters">(default = L' ') - what character to ignore</param>
		/// <returns>the word</returns>
		template <typename CharT>
		std::basic_string<CharT> FindNextWord(const std::basic_string<CharT>& string, const int& startPosition, const CharT& delimiter = L' ')
		{
			std::basic_string<CharT> tmpString;
			int tmpInt;
			return FindNextWord<CharT>(string, startPosition, &tmpString, &tmpInt, delimiter);
		}
	#pragma endregion

	#pragma region FindNthWord
		/// <summary>
		/// finds Nth word from position
		/// </summary>
		/// <typeparam name="CharT">- string type</typeparam>
		/// <param name="string">- the string to look through</param>
		/// <param name="startPosition">- position to start looking from</param>
		/// <param name="wordCount">- how many words to go over</param>
		/// <param name="word">- pointer to string which will get modified to the word found</param>
		/// <param name="wordStartPosition">- pointer to int which will get modified with start position of the word</param>
		/// <param name="delimiters">(default = L' ') - what character to ignore</param>
		/// <returns>the word</returns>
		template <typename CharT>
		std::basic_string<CharT> FindNthWord(const std::basic_string<CharT>& string, const int& startPosition, const int& wordCount, std::basic_string<CharT>* word, int* wordStartPosition, const CharT& delimiter = L' ')
		{
			FindNextWord<CharT>(string, startPosition, word, wordStartPosition, delimiter);
			for (int i = 0; i < wordCount; i++)
			{
				FindNextWord<CharT>(string, NosLib::Cast::Cast<int>((*wordStartPosition) + word->length()), word, wordStartPosition, delimiter);
			}
			return *word;
		}

		/// <summary>
		/// finds Nth word from position without pointer returns
		/// </summary>
		/// <typeparam name="CharT">- string type</typeparam>
		/// <param name="string">- the string to look through</param>
		/// <param name="startPosition">- position to start looking from</param>
		/// <param name="wordCount">- how many words to go over</param>
		/// <param name="delimiters">(default = ' ') - what character to ignore</param>
		/// <returns>the word</returns>
		template <typename CharT>
		std::basic_string<CharT> FindNthWord(const std::basic_string<CharT>& string, const int& startPosition, const int& wordCount, const CharT& delimiter = L' ')
		{
			std::basic_string<CharT> tmpString;
			int tmpInt;
			FindNthWord<CharT>(string, startPosition, wordCount, &tmpString, &tmpInt, delimiter);
			return tmpString;
		}
	#pragma endregion

	#pragma region SubstringContainCount
		/// <summary>
		/// Counts the amount of times a substring is contained inside another string
		/// </summary>
		/// <typeparam name="CharT">- string type template</typeparam>
		/// <param name="string">- main string</param>
		/// <param name="substring">- substring to be searched for</param>
		/// <returns>count of amount of times substring is in string</returns>
		template <typename CharT>
		int SubstringContainCount(const std::basic_string<CharT>& string, const std::basic_string<CharT>& substring)
		{
			int count = 0;
			size_t pos = 0;
			while ((pos = string.find(substring, pos)) != std::basic_string<CharT>::npos) {
				count++;
				pos += substring.length();
			}

			return count;
		}
	#pragma endregion

		/// <summary>
		/// removes whitespace characters either side of the string
		/// </summary>
		/// <typeparam name="CharT">- string type template</typeparam>
		/// <param name="string">- string to remove from</param>
		/// <param name="whitespace">(default = " \t") - whitespace characters</param>
		/// <returns>trimmed string</returns>
		template <typename CharT>
		std::basic_string<CharT> Trim(const std::basic_string<CharT>& string, const std::basic_string<CharT>& whitespace = NosLib::String::ConvertString < CharT, wchar_t>(L" \t"))
		{
			size_t strBegin = string.find_first_not_of(whitespace);
			if (strBegin == std::basic_string<CharT>::npos)
			{
				return NosLib::String::ConvertString<CharT, wchar_t>(L""); // no content
			}

			return string.substr(strBegin, string.find_last_not_of(whitespace) - strBegin + 1);
		}

		/// <summary>
		/// Reduces all whitespaces to just 1 together. "abc     edf" -> "abc edf"
		/// </summary>
		/// <typeparam name="CharT">- string type template</typeparam>
		/// <param name="string">- string to reduce from</param>
		/// <param name="fill">(default = " ") - what to replace whitespaces with</param>
		/// <param name="whitespace">(default = " \t") - whitespace character</param>
		/// <returns>reduced string</returns>
		template <typename CharT>
		std::basic_string<CharT> Reduce(const std::basic_string<CharT>& string, const std::basic_string<CharT>& fill = NosLib::String::ConvertString<CharT, wchar_t>(L" "), const std::basic_string<CharT>& whitespace = NosLib::String::ConvertString < CharT, wchar_t>(L" \t"))
		{
			// trim first
			std::basic_string<CharT> result = Trim<CharT>(string, whitespace);

			// replace sub ranges
			size_t beginSpace = result.find_first_of(whitespace);
			while (beginSpace != std::wstring::npos)
			{
				result.replace(beginSpace, result.find_first_not_of(whitespace, beginSpace) - beginSpace, fill);

				beginSpace = result.find_first_of(whitespace, beginSpace + fill.length());
			}

			return result;
		}
	}
}
#endif