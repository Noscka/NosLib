#ifndef _VECTOR_NOSSTDLIB_HPP_
#define _VECTOR_NOSSTDLIB_HPP_

#include "String.hpp"

#include <string>
#include <format>

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
		class VectorD2
		{
		public:
			int X;
			int Y;

			/// <summary>
			/// Creates a 2 dimentional vector object
			/// </summary>
			/// <param name="x">- x coord</param>
			/// <param name="y">- y coord</param>
			VectorD2(int x, int y)
			{
				X = x;
				Y = y;
			}

			/// <summary>
			/// + operator
			/// </summary>
			/// <param name="addingObject">- the second object to add</param>
			/// <returns>An object of the other objects added together</returns>
			VectorD2 operator+(VectorD2& addingObject)
			{
				return VectorD2((this->X + addingObject.X), (this->Y + addingObject.Y));
			}

			/// <summary>
			/// Converts Vector to string
			/// </summary>
			/// <typeparam name="CharT">- what character type to convert to</typeparam>
			/// <returns>the string version of the vector</returns>
			template <typename CharT>
			std::basic_string<CharT> str()
			{
				return NosStdLib::String::ConvertStringTypes<wchar_t, CharT>(std::format(L"({}, {})", this->X, this->Y));
			}
		};

		/// <summary>
		/// A 3 dimentional vector
		/// </summary>
		class VectorD3
		{
		public:
			int X;
			int Y;
			int Z;

			/// <summary>
			/// Creates a 3 dimentional vector object
			/// </summary>
			/// <param name="x">- x coord</param>
			/// <param name="y">- y coord</param>
			/// <param name="z">- z coord</param>
			VectorD3(int x, int y, int z)
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
			VectorD3 operator+(VectorD3& addingObject)
			{
				return VectorD3((this->X + addingObject.X), (this->Y + addingObject.Y), (this->Z + addingObject.Z));
			}

			/// <summary>
			/// Converts Vector to string
			/// </summary>
			/// <typeparam name="CharT">- what character type to convert to</typeparam>
			/// <returns>the string version of the vector</returns>
			template <typename CharT>
			std::basic_string<CharT> str()
			{
				return NosStdLib::String::ConvertStringTypes<wchar_t, CharT>(std::format(L"({}, {}, {})", this->X, this->Y, this->Z));
			}
		};
	}
}
#endif