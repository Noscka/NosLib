#ifndef _DYNAMICARRAY_HPP_
#define _DYNAMICARRAY_HPP_

#include <iostream>

namespace NosStdLib
{
	/* TODO: Add modes (such as double mode were the array doubles in size instead of increasing by stepsize) */

	/// <summary>
	/// Class which allows for making arrays that can self increase on data overflow
	/// </summary>
	/// <typeparam name="ArrayDataType">- datatype for the array</typeparam>
	template<class ArrayDataType>
	class DynamicArray
	{
	private:
		int ArraySize;				// Array starting size and the size after it is resized
		ArrayDataType* MainArray;	// Pointer to Array
		int ArrayIndexPointer;		// keeps track amount of objects in array
		int ArrayStepSize;			// how much the array will get increased by when it reaches the limit

		typedef ArrayDataType* iterator;
		typedef const ArrayDataType* const_iterator;
	public:
	#pragma region Constructors
		/// <summary>
		/// Constructor with starting size and step size params for custom objects
		/// </summary>
		/// <param name="StartSize"> - Starting size of the array</param>
		/// <param name="StepSize"> - how much the array will increase each time it reaches the limit</param>
		DynamicArray(int startSize, int stepSize)
		{
			ArraySize = startSize;
			ArrayStepSize = stepSize;

			// ! DO NOT CHANGE !
			ArrayIndexPointer = 0;
			MainArray = new ArrayDataType[ArraySize]();
		}

		/// <summary>
		/// Constructor to set all the variables
		/// </summary>
		DynamicArray()
		{
			/* TODO: make this constructer run the other one. Requires testing
			most likely is:

			*this = DynamicArray(10,10);
			
			*/
			
			/* Default values, can be changed */
			ArraySize = 10;
			ArrayStepSize = 10;

			/* !DO NOT CHANGE ! */
			ArrayIndexPointer = 0;
			MainArray = new ArrayDataType[ArraySize]();
		}

		/// Destroy array contained in object
		~DynamicArray()
		{
			delete[] MainArray;
		}
	#pragma endregion

	#pragma region MainArray Modification
		/// <summary>
		/// Append single Object
		/// </summary>
		/// <param name="ObjectToAdd"> - Object to add</param>
		void Append(ArrayDataType objectToAdd)
		{
			if (ArrayIndexPointer >= ArraySize) // if Current Index pointer is more then the array size (trying to add to OutOfRange space)
			{
				ArrayDataType* TempArray = new ArrayDataType[ArraySize](); // Create new array which will store the original values

				for (int i = 0; i < ArraySize; i++) // assign/copy all values from Array to Temp
				{
					TempArray[i] = MainArray[i];
				}

				ArraySize += ArrayStepSize; // expand the Array size
				MainArray = new ArrayDataType[ArraySize](); // over ride MainArray with new, bigger, array

				/*
				ArraySize-ArrayStepSize calculates TempArray size
				Copy all values from Temp array to "old" expanded array
				*/
				for (int i = 0; i < ArraySize - ArrayStepSize; i++)
				{
					MainArray[i] = TempArray[i];

				}

				delete[] TempArray;
			}

			MainArray[ArrayIndexPointer] = objectToAdd;
			ArrayIndexPointer++;
		}

		/// <summary>
		/// Appends another array to the DynamicArray
		/// </summary>
		/// <param name="ArrayToAdd"> - the array to append</param>
		/// <param name="size"> - size of the array</param>
		/// <param name="includeEscape">(default = false) -  !TEXT DATA TYPES ONLY! if escape characters should be appended as well</param>
		void ArrayAppend(ArrayDataType arrayToAdd[], int size, bool includeEscape = false)
		{
			if (std::is_same<ArrayDataType, std::string>::value || std::is_same<ArrayDataType, char>::value) // For Text types only
			{
				for (int i = 0; i < size; ++i) // for loop to get and append all character
				{
					if (includeEscape) // if IncludeEscape is true, append all character
					{
						Append(arrayToAdd[i]); // Append character to array
					}
					else if (!(arrayToAdd[i] == 0)) // if IncludeEscape is false, include all chars that aren't null
					{
						Append(arrayToAdd[i]); // Append character to array
					}
				}
			}
			else
			{
				for (int i = 0; i < size; ++i) // for loop to get and append all Array Objects
				{
					Append(arrayToAdd[i]);
				}
			}
		}

		/// <summary>
		/// Replaces a a chosen positin with the chosen Object
		/// </summary>
		/// <param name="ReplaceObject"> - Object to place in the position</param>
		/// <param name="position"> - position to put the Object in</param>
		void Replace(ArrayDataType replaceObject, int position)
		{
			if (position >= (ArrayIndexPointer - 1) || position < 0)// check if the position to remove is in array range
			{
				throw std::out_of_range("position was out of range of the array");
				return;
			}

			MainArray[position] = replaceObject;
		}

		/// <summary>
		/// Remove and move all Object infront, back 1 spot
		/// </summary>
		/// <param name="position"> - Position to remove</param>
		void Remove(int position)
		{
			if (position >= (ArrayIndexPointer - 1) || position < 0)// check if the position to remove is in array range
			{
				throw std::out_of_range("position was out of range of the array");
				return;
			}

			for (int i = position; i < (ArrayIndexPointer - 1); i++) // moving all back
			{
				MainArray[i] = MainArray[i + 1];
			}
			MainArray[ArrayIndexPointer - 1] = NULL; // make last character blank
			ArrayIndexPointer--;
		}

		/// <summary>
		/// Clear the dynamic array to the original size
		/// </summary>
		void Clear()
		{
			/* TODO: Return to original size */
			ArrayIndexPointer = 0;
			MainArray = new ArrayDataType[ArraySize]();
		}
	#pragma endregion

	#pragma region Variable Returns
		/// <summary>
		/// Return array contained in the object
		/// </summary>
		/// <returns>Object array</returns>
		ArrayDataType* GetArray()
		{
			return MainArray;
		}

		/// <summary>
		/// Returns the max array size (won't be the current index)
		/// </summary>
		/// <returns>int of current array size</returns>
		int GetArraySize()
		{
			return ArraySize;
		}

		/// <summary>
		/// Returns the current index/amount of objects in the array
		/// </summary>
		/// <returns>index/amount of objects in array</returns>
		int GetArrayIndexPointer()
		{
			return ArrayIndexPointer;
		}

		/// <summary>
		/// Returns the steps size
		/// </summary>
		/// <returns>step size</returns>
		int GetArrayStepSize()
		{
			return ArrayStepSize;
		}
	#pragma endregion

	#pragma region For Loop Functions
		// For loop range-based function
		iterator begin() { return &MainArray[0]; }
		const_iterator begin() const { return &MainArray[0]; }
		iterator end() { return &MainArray[ArrayIndexPointer]; }
		const_iterator end() const { return &MainArray[ArrayIndexPointer]; }
	#pragma endregion

	#pragma region Operators
		/// <summary>
		/// What to do incase of << operator with wostream
		/// </summary>
		/// <param name="os">- output stream</param>
		/// <param name="Array">- the array object</param>
		/// <returns></returns>
		friend std::wostream& operator<<(std::wostream& os, const DynamicArray& MainArray)
		{
			if (std::is_same<ArrayDataType, std::wstring>::value || std::is_same<ArrayDataType, wchar_t>::value) /* if the array contained is either string, wstring, char or wchar. output as normal */
			{
				os << MainArray.MainArray;
			}
			else  /* if type is any other "list" the array with comman */
			{
				for (int i = 0; i < MainArray.ArrayIndexPointer; i++)
				{
					os << MainArray.MainArray[i];

					if (!(i == MainArray.ArrayIndexPointer - 1))
					{
						os << ", ";
					}
				}
			}
			return os;
		}

		/// <summary>
		/// What to do incase of << operator with ostream
		/// </summary>
		/// <param name="os">- output stream</param>
		/// <param name="Array">- the array object</param>
		/// <returns></returns>
		friend std::ostream& operator<<(std::ostream& os, const DynamicArray& MainArray)
		{
			if (std::is_same<ArrayDataType, std::string>::value || std::is_same<ArrayDataType, char>::value) /* if the array contained is either string, wstring, char or wchar. output as normal */
			{
				os << MainArray.MainArray;
			}
			else  /* if type is any other "list" the array with comman */
			{
				for (int i = 0; i < MainArray.ArrayIndexPointer; i++)
				{
					os << MainArray.MainArray[i];

					if (!(i == MainArray.ArrayIndexPointer - 1))
					{
						os << ", ";
					}
				}
			}
			return os;
		}

		/// <summary>
		/// [] operator which acts the same as the Array [] operator
		/// </summary>
		/// <param name="position">- position of the value wanted</param>
		/// <returns>value in the position</returns>
		ArrayDataType& operator[](int position)
		{
			return MainArray[position];
		}
	#pragma endregion
	};
}

#endif
