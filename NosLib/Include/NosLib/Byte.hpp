#ifndef _BINARYMATH_NOSLIB_HPP_
#define _BINARYMATH_NOSLIB_HPP_

#include <cmath>
#include <algorithm>

namespace NosLib
{
	/// <summary>
	/// namespace which contains items related to binary maths
	/// </summary>
	namespace Byte
	{
		/// <summary>
		/// A function made to calculate the max value that can be stored in the given bit count
		/// </summary>
		/// <param name="bitCount">- the amout of bits</param>
		/// <returns>the max value that can be stored</returns>
		inline constexpr int MaxBitValue(const int& bitCount)
		{
			int maxValue = 0;
			for (int i = 0; bitCount > i; i++)
			{
				maxValue += static_cast<int>(pow(2, i));
			}
			return maxValue;
		}

		/// <summary>
		/// A function made to calculate the max value that can be stored in the given byte count
		/// </summary>
		/// <param name="byteCount">- the amount of bytes</param>
		/// <returns>the max value that can be stored</returns>
		inline constexpr int MaxByteValue(const int& byteCount)
		{
			return MaxBitValue(byteCount * 8);
		}


		/// <summary>
		/// converts an Arithematic to byte (char array)
		/// </summary>
		/// <typeparam name="numType">- type</typeparam>
		/// <param name="intIn">- number in</param>
		/// <param name="byteOut">- char array out</param>
		template<class numType>
		inline constexpr void ArithematicToByte(const numType& intIn, char** byteOut)
		{
			static_assert(std::is_arithmetic_v<numType>, "Type has to be either an Integer or Floating point");

			(*byteOut) = new char[sizeof(numType)];
			std::copy(static_cast<const char*>(&intIn), static_cast<const char*>(&intIn) + sizeof(numType), (*byteOut));
		}

		/// <summary>
		/// converts an bytes (char array) to Arithematic 
		/// </summary>
		/// <typeparam name="numType">- type</typeparam>
		/// <param name="byte">- bytes to convert to type</param>
		/// <returns>converted value of type</returns>
		template<class numType>
		inline constexpr numType ByteToArithematic(char* byte)
		{
			numType out = 0;
			std::copy(byte, byte + sizeof(numType), static_cast<char*>(&out));
			return out;
		}

		/// <summary>
		/// Takes in any 2 types (can be the same), converts them to bytes and compares byte by byte
		/// </summary>
		/// <typeparam name="type1">- type for in1</typeparam>
		/// <typeparam name="type2">- type for in2</typeparam>
		/// <param name="in1">- value for in1</param>
		/// <param name="in2">- value for in2</param>
		/// <param name="count">(default = 1) - for arrays, amount of objects in array</param>
		/// <returns>true if the same, false if different</returns>
		template<class type1, class type2>
		inline constexpr bool ByteCompare(const type1& in1, const type2& in2, const size_t& count = 1)
		{
			char in1Byte[count * sizeof(type1)];
			char in2Byte[count * sizeof(type2)];

			std::copy(static_cast<const char*>(&in1), static_cast<const char*>(&in1) + (sizeof(type1) * count), in1Byte);
			std::copy(static_cast<const char*>(&in2), static_cast<const char*>(&in2) + (sizeof(type2) * count), in2Byte);


			for (int i = 0; i < count * std::min(sizeof(type1), sizeof(type2)); i++)
			{
				if (in1Byte[i] != in2Byte[i])
				{
					return false;
				}
			}

			return true;
		}
	}
}

#endif