#ifndef _BINARYMATH_NosLib_HPP_
#define _BINARYMATH_NosLib_HPP_

#include <cmath>
#include <algorithm>

#include "Cast.hpp"
#include "TypeTraits.hpp"

namespace NosLib
{
	/// <summary>
	/// namespace which contains items related to binary maths
	/// </summary>
	namespace BinaryMaths
	{
		/// <summary>
		/// A function made to calculate the max value that can be stored in the given bit count
		/// </summary>
		/// <param name="bitCount">- the amout of bits</param>
		/// <returns>the max value that can be stored</returns>
		inline int MaxBitValue(const int& bitCount)
		{
			int maxValue = 0;
			for (int i = 0; bitCount > i; i++)
			{
				maxValue += NosLib::Cast<int>(pow(2, i));
			}
			return maxValue;
		}

		/// <summary>
		/// A function made to calculate the max value that can be stored in the given byte count
		/// </summary>
		/// <param name="byteCount">- the amount of bytes</param>
		/// <returns>the max value that can be stored</returns>
		inline int MaxByteValue(const int& byteCount)
		{
			return MaxBitValue(byteCount * 8);
		}

		template<class numType>
		inline void ArithematicToByte(const numType& intIn, char** byteOut)
		{
			static_assert(std::is_arithmetic_v<numType>, "Type has to be either an Integer or Floating point");

			(*byteOut) = new char[sizeof(numType)];
			std::copy(NosLib::Cast<const char*>(&intIn), NosLib::Cast<const char*>(&intIn) + sizeof(numType), (*byteOut));
		}

		template<class numType>
		inline numType ByteToArithematic(char* byte)
		{
			numType out = 0;
			std::copy(byte, byte + sizeof(numType), NosLib::Cast<char*>(&out));
			return out;
		}
	}
}

#endif