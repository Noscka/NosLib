#ifndef _DIMENTIONVECTOR_NosLib_HPP_
#define _DIMENTIONVECTOR_NosLib_HPP_

#include "String.hpp"

#include <string>
#include <format>
#include <type_traits>

namespace NosLib
{
	/// <summary>
	/// namespace that contains items on vectors
	/// </summary>
	namespace Vector
	{
		/// <summary>
		/// A 2 dimensional vector
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
			/// Creates a 2 dimensional vector object with x,y
			/// </summary>
			/// <param name="x">- x coord</param>
			/// <param name="y">- y coord</param>
			VectorD2(const ArithmeticType& x, const ArithmeticType& y)
			{
				X = x;
				Y = y;
			}

			/// <summary>
			/// Creates a 2 dimensional vector object with COORD struct
			/// </summary>
			/// <param name="coordPoint">- the coord to use</param>
			VectorD2(const COORD& coordPoint)
			{
				X = coordPoint.X;
				Y = coordPoint.Y;
			}

			/// <summary>
			/// Creates a 2 dimensional vector object with POINT struct
			/// </summary>
			/// <param name="point">- the point to use</param>
			VectorD2(const POINT& point)
			{
				X = point.x;
				Y = point.y;
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
			/// Convert to POINT
			/// </summary>
			operator POINT()
			{
				return POINT(X, Y);
			}

			/// <summary>
			/// Converts Vector to string
			/// </summary>
			/// <typeparam name="CharT">- what character type to convert to</typeparam>
			/// <returns>the string version of the vector</returns>
			template <typename CharT>
			std::basic_string<CharT> str()
			{
				return NosLib::String::ConvertString<CharT, wchar_t>(std::format(L"({}, {})", this->X, this->Y));
			}
		};

		/// <summary>
		/// A 3 dimensional vector
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
			/// Creates a 3 dimensional vector object
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
				return NosLib::String::ConvertString<CharT, wchar_t>(std::format(L"({}, {}, {})", this->X, this->Y, this->Z));
			}
		};
	}

	namespace Dimension
	{
		/// <summary>
		/// class which holds 2 points which are used for drawing a box
		/// </summary>
		template<typename ArithmeticType>
		class DimensionD2
		{
			static_assert(std::is_arithmetic_v<ArithmeticType>, "Vector datatype has to be an arithmetic type");
		public:
			NosLib::Vector::VectorD2<ArithmeticType> PointOne;	/* Top Left Coord vector */
			NosLib::Vector::VectorD2<ArithmeticType> PointTwo;	/* Bottom Right Coord vector */
			NosLib::Vector::VectorD2<ArithmeticType> Offset;	/* Vector which holds offset values */

			DimensionD2() {}

			/// <summary>
			/// Create object with int coords
			/// </summary>
			/// <param name="pointOneX">- Top X</param>
			/// <param name="pointOneY">- Left Y</param>
			/// <param name="pointTwoX">- Bottom X</param>
			/// <param name="pointTwoY">- Right Y</param>
			/// <param name="offsetX">(default = 0) - amount that gets added/taken away from X</param>
			/// <param name="offsetY">(default = 0) - amount that gets added/taken away from Y</param>
			DimensionD2(const ArithmeticType& pointOneX, const ArithmeticType& pointOneY, const ArithmeticType& pointTwoX, const ArithmeticType& pointTwoY, const ArithmeticType& offsetX = 0, const ArithmeticType& offsetY = 0)
			{
				UpdateDimension(pointOneX, pointOneY, pointTwoX, pointTwoY, offsetX, offsetY);
			}

			/// <summary>
			/// Create object with Vector objects
			/// </summary>
			/// <param name="pointOne">- Top Left</param>
			/// <param name="pointTwo">- Bottom Right</param>
			/// <param name="Offset">(default = (0,0))- vector which containts amount that will get added/taken away from in calculations</param>
			DimensionD2(const NosLib::Vector::VectorD2<ArithmeticType>& pointOne, const NosLib::Vector::VectorD2<ArithmeticType>& pointTwo, const NosLib::Vector::VectorD2<ArithmeticType>& offset = NosLib::Vector::VectorD2<ArithmeticType>(0, 0))
			{
				UpdateDimension(pointOne, pointTwo, offset);
			}

			/// <summary>
			/// update object with int coords
			/// </summary>
			/// <param name="pointOneX">- Top X</param>
			/// <param name="pointOneY">- Left Y</param>
			/// <param name="pointTwoX">- Bottom X</param>
			/// <param name="pointTwoY">- Right Y</param>
			/// <param name="offsetX">(default = 0) - amount that gets added/taken away from X</param>
			/// <param name="offsetY">(default = 0) - amount that gets added/taken away from Y</param>
			void UpdateDimension(const ArithmeticType& pointOneX, const ArithmeticType& pointOneY, const ArithmeticType& pointTwoX, const ArithmeticType& pointTwoY, const ArithmeticType& offsetX = 0, const ArithmeticType& offsetY = 0)
			{
				PointOne = NosLib::Vector::VectorD2<ArithmeticType>(pointOneX, pointOneY);
				PointTwo = NosLib::Vector::VectorD2<ArithmeticType>(pointTwoX, pointTwoY);
				Offset = NosLib::Vector::VectorD2<ArithmeticType>(offsetX, offsetY);
			}

			/// <summary>
			/// update object with Vector objects
			/// </summary>
			/// <param name="pointOne">- Top Left</param>
			/// <param name="pointTwo">- Bottom Right</param>
			/// <param name="Offset">(default = (0,0))- vector which containts amount that will get added/taken away from in calculations</param>
			void UpdateDimension(const NosLib::Vector::VectorD2<ArithmeticType>& pointOne, const NosLib::Vector::VectorD2<ArithmeticType>& pointTwo, const NosLib::Vector::VectorD2<ArithmeticType>& offset = NosLib::Vector::VectorD2<ArithmeticType>(0, 0))
			{
				PointOne = pointOne;
				PointTwo = pointTwo;
				Offset = offset;
			}

			/// <summary>
			/// Checks if position passed is inside box
			/// </summary>
			/// <param name="position">- position to check for</param>
			/// <returns>true if position is inside, false if outside</returns>
			bool CheckIfPositionInside(const NosLib::Vector::VectorD2<ArithmeticType>& position)
			{
				return (position.X >= PointOne.X && position.Y >= PointOne.Y) &&
						(position.X <= PointTwo.X && position.Y <= PointTwo.Y);
			}

			/// <summary>
			/// calculate size (pointeTwo-pointOne)
			/// </summary>
			/// <param name="offset">(default = true) - if calculation should take offset into considuration</param>
			/// <returns>VectorD2 with X and Y being sizes</returns>
			NosLib::Vector::VectorD2<ArithmeticType> CalculateSize(const bool& offset = true)
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
			NosLib::Vector::VectorD2<ArithmeticType> CalculateSize(const NosLib::Vector::VectorD2<ArithmeticType>& offset)
			{
				return (PointTwo - PointOne) + offset;
			}
		};

	}
}
#endif