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
		class ArrayClass
		{
		private:
			std::wstring ClassName;
			int ClassValue;
		public:
			ArrayClass()
			{

			}

			ArrayClass(std::wstring className, int classValue)
			{
				ClassName = className;
				ClassValue = classValue;
			}

			std::wstring GetName()
			{
				return ClassName;
			}

			int GetValue()
			{
				return ClassValue;
			}
		};

		class TestClass
		{
		private:
			NosStdLib::DynamicArray<ArrayClass*> SomeArray;
		public:
			TestClass()
			{
				SomeArray = NosStdLib::DynamicArray<ArrayClass*>(3,2);
			}

			void AppendToClass(ArrayClass *object)
			{
				SomeArray.Append(object);
			}
		};
	}
}

#endif