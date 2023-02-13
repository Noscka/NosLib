#ifndef _DIMENTIONVECTOR_NOSSTDLIB_HPP_
#define _DIMENTIONVECTOR_NOSSTDLIB_HPP_

#include "String.hpp"

#include <string>
#include <format>
#include <type_traits>

namespace NosStdLib
{
	/// <summary>
	/// namespace that containts items on vectors
	/// </summary>
	namespace Vector
	{
		/// <summary>
		/// A 2 dimentional vector
		/// </summary>
		template<typename ArithmeticType>
		class VectorD2
		{
			static_assert(std::is_arithmetic_v<ArithmeticType>, "Vector datatype has to be an arithmetic type");
		public:
			ArithmeticType X;
			ArithmeticType Y;

			/// <summary>
			/// Default VectorD2 Constructor
			/// </summary>
			VectorD2(){}

			/// <summary>
			/// Creates a 2 dimentional vector object
			/// </summary>
			/// <param name="x">- x coord</param>
			/// <param name="y">- y coord</param>
			VectorD2(const ArithmeticType& x, const ArithmeticType& y)
			{
				X = x;
				Y = y;
			}

			/// <summary>
			/// + operator
			/// </summary>
			/// <param name="addingObject">- the second object to add</param>
			/// <returns>An object of the other objects added together</returns>
			VectorD2<ArithmeticType> operator+(const VectorD2<ArithmeticType>& addingObject)
			{
				return VectorD2((this->X + addingObject.X), (this->Y + addingObject.Y));
			}

			/// <summary>
			/// - operator
			/// </summary>
			/// <param name="subtractingObject">- the second object to subtract</param>
			/// <returns>An object of the other objects subtracted together</returns>
			VectorD2<ArithmeticType> operator-(const VectorD2<ArithmeticType>& subtractingObject)
			{
				return VectorD2((this->X - subtractingObject.X), (this->Y - subtractingObject.Y));
			}

			/// <summary>
			/// == operator
			/// </summary>
			/// <param name="comparingObject">- second object to compare against</param>
			/// <returns>if vectors represent the same position or not</returns>
			bool operator==(const VectorD2<ArithmeticType>& comparingObject) const
			{
				return (this->X == comparingObject.X && this->Y == comparingObject.Y);
			}

			/// <summary>
			/// Convert to COORD
			/// </summary>
			operator COORD()
			{
				return COORD(X,Y);
			}

			/// <summary>
			/// Converts Vector to string
			/// </summary>
			/// <typeparam name="CharT">- what character type to convert to</typeparam>
			/// <returns>the string version of the vector</returns>
			template <typename CharT>
			std::basic_string<CharT> str()
			{
				return NosStdLib::String::ConvertString<CharT, wchar_t>(std::format(L"({}, {})", this->X, this->Y));
			}
		};

		/// <summary>
		/// A 3 dimentional vector
		/// </summary>
		template<typename ArithmeticType>
		class VectorD3
		{
			static_assert(std::is_arithmetic_v<ArithmeticType>, "Vector datatype has to be an arithmetic type");
		public:
			ArithmeticType X;
			ArithmeticType Y;
			ArithmeticType Z;

			/// <summary>
			/// Default VectorD3 Constructor
			/// </summary>
			VectorD3() {}

			/// <summary>
			/// Creates a 3 dimentional vector object
			/// </summary>
			/// <param name="x">- x coord</param>
			/// <param name="y">- y coord</param>
			/// <param name="z">- z coord</param>
			VectorD3(const ArithmeticType& x, const ArithmeticType& y, const ArithmeticType& z)
			{
				X = x;
				Y = y;
				Z = z;
			}

			/// <summary>
			/// + operator
			/// </summary>
			/// <param name="addingObject">- the second object to add</param>
			/// <returns>An object of the other objects added together</returns>
			VectorD3<ArithmeticType> operator+(const VectorD3<ArithmeticType>& addingObject)
			{
				return VectorD3((this->X + addingObject.X), (this->Y + addingObject.Y), (this->Z + addingObject.Z));
			}

			/// <summary>
			/// - operator
			/// </summary>
			/// <param name="subtractingObject">- the second object to subtract</param>
			/// <returns>An object of the other objects subtracted together</returns>
			VectorD3<ArithmeticType> operator-(const VectorD3<ArithmeticType>& subtractingObject)
			{
				return VectorD3((this->X - subtractingObject.X), (this->Y - subtractingObject.Y), (this->Z - subtractingObject.Z));
			}

			/// <summary>
			/// == operator
			/// </summary>
			/// <param name="comparingObject">- second object to compare against</param>
			/// <returns>if vectors represent the same position or not</returns>
			bool operator==(const VectorD3<ArithmeticType>& comparingObject) const
			{
				return (this->X == comparingObject.X && this->Y == comparingObject.Y && this->Z == comparingObject.Z);
			}

			/// <summary>
			/// Converts Vector to string
			/// </summary>
			/// <typeparam name="CharT">- what character type to convert to</typeparam>
			/// <returns>the string version of the vector</returns>
			template <typename CharT>
			std::basic_string<CharT> str()
			{
				return NosStdLib::String::ConvertString<CharT, wchar_t>(std::format(L"({}, {}, {})", this->X, this->Y, this->Z));
			}
		};
	}

	namespace Dimention
	{
		/// <summary>
		/// class which holds 2 points which are used for drawing a box
		/// </summary>
		class DimentionsD2
		{
		public:
			NosStdLib::Vector::VectorD2<int16_t> PointOne;	/* Top Left Coord vector */
			NosStdLib::Vector::VectorD2<int16_t> PointTwo;	/* Bottom Right Coord vector */
			NosStdLib::Vector::VectorD2<int16_t> Offset;	/* Vector which holds offset values */

			DimentionsD2() {}

			/// <summary>
			/// Create object with int coords
			/// </summary>
			/// <param name="pointOneX">- Top X</param>
			/// <param name="pointOneY">- Left Y</param>
			/// <param name="pointTwoX">- Bottom X</param>
			/// <param name="pointTwoY">- Right Y</param>
			/// <param name="offsetX">(default = 0) - amount that gets added/taken away from X</param>
			/// <param name="offsetY">(default = 0) - amount that gets added/taken away from Y</param>
			DimentionsD2(const int16_t& pointOneX, const int16_t& pointOneY, const int16_t& pointTwoX, const int16_t& pointTwoY, const int16_t& offsetX = 0, const int16_t& offsetY = 0)
			{
				PointOne = NosStdLib::Vector::VectorD2<int16_t>(pointOneX, pointOneY);
				PointTwo = NosStdLib::Vector::VectorD2<int16_t>(pointTwoX, pointTwoY);
				Offset = NosStdLib::Vector::VectorD2<int16_t>(offsetX, offsetY);
			}

			/// <summary>
			/// Create object with Vector objects
			/// </summary>
			/// <param name="pointOne">- Top Left</param>
			/// <param name="pointTwo">- Bottom Right</param>
			/// <param name="Offset">- vector which containts amount that will get added/taken away from in calculations</param>
			DimentionsD2(const NosStdLib::Vector::VectorD2<int16_t>& pointOne, const NosStdLib::Vector::VectorD2<int16_t>& pointTwo, const NosStdLib::Vector::VectorD2<int16_t>& offset = NosStdLib::Vector::VectorD2<int16_t>(0, 0))
			{
				PointOne = pointOne;
				PointTwo = pointTwo;
				Offset = offset;
			}

			/// <summary>
			/// calculate size (pointeTwo-pointOne)
			/// </summary>
			/// <param name="offset">(default = true) - if calculation should take offset into considuration</param>
			/// <returns>VectorD2 with X and Y being sizes</returns>
			NosStdLib::Vector::VectorD2<int16_t> CalculateSize(const bool& offset = true)
			{
				if (offset)
				{
					return (PointTwo - PointOne) + Offset;
				}
				else
				{
					return (PointTwo - PointOne);
				}
			}

			/// <summary>
			/// calculate size with custom offset (pointeTwo-pointOne)
			/// </summary>
			/// <param name="offset">- the offset which will get used in the calculation</param>
			/// <returns>VectorD2 with X and Y being sizes that are offset</returns>
			NosStdLib::Vector::VectorD2<int16_t> CalculateSize(const NosStdLib::Vector::VectorD2<int16_t>& offset)
			{
				return (PointTwo - PointOne) + offset;
			}
		};

	}
}
#endif