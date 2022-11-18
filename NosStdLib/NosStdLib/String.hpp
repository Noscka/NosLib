#ifndef _STRING_NOSSTDLIB_HPP_
#define _STRING_NOSSTDLIB_HPP_

#include <Windows.h>
#include <stringapiset.h>
#include <string>
#include <sstream>
#include <vector>
#include <minmax.h>
#include <algorithm>
#include <iterator>
#include <set>

namespace NosStdLib
{
	/// <summary>
	/// namespace for items which are related to strings (both string and wstring)
	/// </summary>
	namespace String
	{
	#pragma region String Conversion
		/// <summary>
		/// Converts any string type to any other string type
		/// </summary>
		/// <typeparam name="StringFrom">- the string input type</typeparam>
		/// <typeparam name="StringTo">- the string output type</typeparam>
		/// <param name="strIn">- string input</param>
		/// <param name="narrowChars">- If the function should narrow down characters or keep as is (might result in 1 char being expand to 2)</param>
		/// <returns>converted string</returns>
		template<typename StringFrom, typename StringTo>
		std::basic_string<StringTo> ConvertStringTypes(const std::basic_string<StringFrom>& strIn, const bool& narrowChars = false)
		{
			if constexpr (std::is_same_v<StringFrom, StringTo>) /* if same character type, then just return string */
			{
				return strIn;
			}
			else /* else, check what type of conversion to do */
			{
				if (narrowChars) /* if true, narrow down characters to prevent 2 characters getting created from 1 wide character */
				{
					int maxValue = NosStdLib::Global::BinaryMaths::MaxByteValue(sizeof(StringTo));
					std::basic_string<StringTo> strOut;

					for (int i = 0; strIn.size() >  i; i++)
					{
						if ((int)(strIn[i]) > maxValue)
						{
							continue;
						}
						
						std::copy(&strIn[i], &strIn[i], std::back_insert_iterator(strOut));
					}

					return strOut;
				}
				else /* else just convert it without narrowing */
				{
					std::basic_string<StringTo> strOut;
					std::copy(strIn.begin(), strIn.end(), std::back_inserter(strOut));
					return strOut;
				}
			}
		}

		#define ToString ConvertStringTypes<wchar_t, char> 
		#define ToWstring ConvertStringTypes<char, wchar_t>
	#pragma endregion


	#pragma region IsNumber
		/// <summary>
		/// Check if wstring is number (with or without signs)
		/// </summary>
		/// <typeparam name="CharT">- string type</typeparam>
		/// <param name="str">- string to check</param>
		/// <param name="allowSigns">(default = true) - if it should allow signs at the start of the string (-123 or +123)</param>
		/// <returns>if string is valid number</returns>
		template <typename CharT>
		bool IsNumber(const std::basic_string<CharT>& str, bool allowSigns = true)
		{
			/* Iterator int, allows for changing start position */
			int Iteration = 0;
			if (allowSigns)
			{
				/* if allowSigns is true, check if first character is either - or + or a number */
				if ((str[0] != NosStdLib::String::ConvertStringTypes<wchar_t, CharT>(L'-') && str[0] != NosStdLib::String::ConvertStringTypes<wchar_t, CharT>(L'+')) && !std::isdigit(str[0]))
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
		std::vector<std::basic_string<CharT>>* Split(std::vector<std::basic_string<CharT>>* result, std::basic_string<CharT>* input, const CharT& delimiter = L' ')
		{
			std::basic_string<CharT> tmp;
			std::basic_stringstream<CharT> ss(*input);

			while (std::getline(ss, tmp, delimiter))
			{
				result->push_back(tmp);
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
		/// <param name="input">- wstring to center</param>
		/// <param name="all">(default = true) - if it should center just first line or all lines</param>
		/// <returns>centered string</returns>
		template <typename CharT>
		std::basic_string<CharT> CenterString(HANDLE consoleHandle, std::basic_string<CharT> input, bool rightPadding = false, bool all = true)
		{
			CONSOLE_SCREEN_BUFFER_INFO csbi; /* TODO: change from using consoleHandle or add overload to use ConsoleSizeStruct */
			GetConsoleScreenBufferInfo(consoleHandle, &csbi);
			int columns = csbi.srWindow.Right - csbi.srWindow.Left + 1;
			if (all)
			{
				std::vector<std::basic_string<CharT>> inputSplit;
				std::basic_string<CharT> output;
				NosStdLib::String::Split<CharT>(&inputSplit, &input, L'\n');

				for (std::basic_string<CharT> Singleinput : inputSplit)
				{
					output += (std::basic_string<CharT>(((columns / 2) - Singleinput.size() / 2), ' ') + Singleinput + (rightPadding ? std::wstring(max((columns - (Singleinput.size() + ((columns / 2) - Singleinput.size() / 2))), 0), L' ') : L"") + (CharT)(L'\n'));
				}

				return output;
			}
			else
			{
				return (std::basic_string<CharT>(((columns / 2) - input.size() / 2), ' ') + input + (rightPadding ? std::wstring(max((columns - (input.size() + ((columns / 2) - input.size() / 2))), 0), L' ') : L"") + (CharT)(L'\n'));
			}
		}

		/// <summary>
		/// Center wstring 
		/// </summary>
		/// <typeparam name="CharT">- string type</typeparam>
		/// <param name="input">- wstring to center</param>
		/// <param name="all">(default = true) - if it should center just first line or all lines</param>
		/// <returns>centered wstring</returns>
		template <typename CharT>
		std::basic_string<CharT> CenterString(std::basic_string<CharT> input, bool rightPadding = false, bool all = true)
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
				FindNextWord<CharT>(string, ((*wordStartPosition) + word->length()), word, wordStartPosition, delimiter);
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
		/// <param name="delimiters">(default = L' ') - what character to ignore</param>
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
	}
}
#endif