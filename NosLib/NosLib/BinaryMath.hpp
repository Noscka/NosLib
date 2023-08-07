#ifndef _BINARYMATH_NosLib_HPP_
#define _BINARYMATH_NosLib_HPP_

#include <math.h>

#include "Cast.hpp"

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
	}
}

#endif