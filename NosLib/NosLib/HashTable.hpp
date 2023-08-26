#ifndef _HASHTABLE_NOSLIB_HPP_
#define _HASHTABLE_NOSLIB_HPP_

#include "TypeTraits.hpp"
#include "Pointers.hpp"

#include <type_traits>
#include <algorithm>
#include <functional>
#include <cstdio>

namespace NosLib
{
	template<class HashTableType>
	class HashTableObjectContainer
	{
	public:
		HashTableType Object;
		HashTableObjectContainer<HashTableType>* Next = nullptr;

		HashTableObjectContainer() {}

		HashTableObjectContainer(HashTableType object)
		{
			Object = object;
		}

		/// <summary>
		/// Add another object to linked list
		/// </summary>
		/// <param name="next"></param>
		void AddNext(HashTableObjectContainer<HashTableType>* next)
		{
			if (Next != nullptr) /* if next isn't empty for this object, pass it on */
			{
				Next->AddNext(next);
			}
			else
			{
				Next = next;
			}
		}

		/// <summary>
		/// Counts amount of objects in list, returns object count and not last index
		/// </summary>
		/// <returns>number of objects in linked array</returns>
		int Count()
		{
			HashTableObjectContainer<HashTableType>* currentIndex = this;

			int count = 1;

			/* just go deeper into the Next path until reach nullptr */
			while (currentIndex->Next != nullptr)
			{
				count++;
				currentIndex = currentIndex->Next;
			}

			return count;
		}

		/// <summary>
		/// Removes next object from linked linked list, and takes the object after that as its own
		/// </summary>
		void RemoveNext()
		{
			/* take the Next object's next object as own. doesn't matter if it is nullptr or not */
			HashTableObjectContainer<HashTableType>* replacement = Next->Next;

			delete Next;
			Next = replacement;
		}

		/// <summary>
		/// [] operator will got the x number forward
		/// </summary>
		/// <param name="position">- position of the value wanted</param>
		/// <returns>value in the position</returns>
		HashTableObjectContainer<HashTableType>& operator[](const int& position)
		{
			HashTableObjectContainer<HashTableType>* indexObject = this;

			/* just go X number times into the linked list */
			for (int i = 0; i < position; i++)
			{
				indexObject = indexObject->Next;
			}

			return (*indexObject);
		}
	};

	template<class HashTableKey, class HashTableType>
	class HashTable
	{
	private:
		using HashTableTypeRoot = NosLib::TypeTraits::remove_all_pointers_t<HashTableType>;
		using HashTableTypeNormalized = std::add_pointer_t<HashTableTypeRoot>;


	protected:
		size_t TableSize;
		HashTableObjectContainer<HashTableType>** MainTable;
		size_t TableStepSize;

		HashTableKey(HashTableTypeRoot::* GetKeyValueFunction)(); /* function used to get key value, so user can use any member in their class */
	public:

		/// <summary>
		/// Constructor
		/// </summary>
		/// <param name="GetKeyValueFunction">- Member function of class used to get the key to hash, so how you can find your object</param>
		/// <param name="startSize">(default = 100) - Array Start Size</param>
		/// <param name="stepSize">(default = 10) - How much the array should increase (CURRENTLY UNUSED)</param>
		HashTable(HashTableKey(HashTableTypeRoot::* getKeyValueFunc)(), const size_t& startSize = 100, const size_t& stepSize = 10)
		{
			GetKeyValueFunction = getKeyValueFunc;
			TableSize = startSize;
			TableStepSize = stepSize;

			MainTable = new HashTableObjectContainer<HashTableType>*[startSize]();
		}

		~HashTable()
		{
			/* if a pointer and not a function, go through all entries and delete */
			if constexpr (std::is_pointer<HashTableType>::value && !std::is_function< NosLib::TypeTraits::remove_all_pointers_t<HashTableType> >::value)
			{
				for (size_t i = 0; i < TableSize; i++)
				{
					delete MainTable[i];
				}
			}
			delete[] MainTable;
		}

		/// <summary>
		/// Inserts object into hash table, has to be pointer
		/// </summary>
		/// <param name="insertObject">- pointer to object</param>
		void Insert(HashTableType insertObject)
		{
			HashTableTypeNormalized normalizedInsertObject = NosLib::Pointers::OneOffRootPointer<HashTableType*>((&insertObject));

			/* do mod on the hash value to convert into a usable position number */
			size_t pos = (std::hash<HashTableKey>{}((normalizedInsertObject->*GetKeyValueFunction)()) % TableSize);
			if (MainTable[pos] != nullptr)
			{
				MainTable[pos]->AddNext(new HashTableObjectContainer<HashTableType>(insertObject));
			}
			else
			{
				MainTable[pos] = new HashTableObjectContainer<HashTableType>(insertObject);
			}
		}

		/// <summary>
		/// Finds object in hash table using key
		/// </summary>
		/// <param name="findKey">- key used to put the hash object into array</param>
		HashTableType* Find(const HashTableKey& findKey)
		{
			/* do mod on the hash value to convert into a usable position number */
			size_t pos = (std::hash<HashTableKey>{}(findKey) % TableSize);

			/* if entry is nullptr, nothing exists under that key */
			if (MainTable[pos] == nullptr)
			{
				return nullptr;
			}

			/* else, that means there is something at pos */
			/* go through all entries in linked list */
			for (int i = 0; i < MainTable[pos]->Count(); i++)
			{
				HashTableType* currentObject = &((*MainTable[pos])[i].Object);

				HashTableTypeNormalized normalizedObject = NosLib::Pointers::OneOffRootPointer<HashTableType*>(currentObject);

				if ((normalizedObject->*GetKeyValueFunction)() == findKey)
				{
					return currentObject;
				}
			}

			/* if got this far, nothing was found */
			return nullptr;
		}

		/// <summary>
		/// Removes object from hash table using key
		/// </summary>
		/// <param name="findKey">- key used to put the hash object into array</param>
		bool Remove(const HashTableKey& findKey)
		{
			/* do mod on the hash value to convert into a usable position number */
			size_t pos = (std::hash<HashTableKey>{}(findKey) % TableSize);

			/* if entry is nullptr, nothing is under key and so remove fails */
			if (MainTable[pos] == nullptr)
			{
				return false;
			}

			/* else, that means there is something at pos */
			/* go through all entries in linked list */
			for (int i = 0; i < MainTable[pos]->Count(); i++)
			{
				HashTableType* currentObject = &((*MainTable[pos])[i].Object);
				HashTableTypeNormalized normalizedObject = NosLib::Pointers::OneOffRootPointer<HashTableType*>(currentObject);

				if ((normalizedObject->*GetKeyValueFunction)() != findKey) /* check if object key matches argument key */
				{
					continue;
				}

				if (i == 0) /* if index is 0, that means its first in list, replace pointer with next */
				{
					HashTableObjectContainer<HashTableType>* replacement = nullptr;

					/* if there are more entries after "next", link them here */
					if ((*this)[pos]->Next != nullptr)
					{
						replacement = (*this)[pos]->Next;
					}

					delete MainTable[pos];
					MainTable[pos] = replacement;
				}
				else /* if index is more then 0, it means its deeper in the list, and so the object before the one we want to remove can "remove next" */
				{
					(*this)[pos][i - 1].RemoveNext();
				}

				return true;
			}

			/* if got this far, nothing was found and so remove failed */
			return false;
		}

		/// <summary>
		/// Returns Overall size of hash table, so how many linked lists it can contain
		/// </summary>
		int GetHashTableSize()
		{
			return TableSize;
		}

		/// <summary>
		/// [] operator which acts the same as the Array [] operator
		/// </summary>
		/// <param name="position">- position of the value wanted</param>
		/// <returns>value in the position</returns>
		HashTableObjectContainer<HashTableType>*& operator[](const int& position)
		{
			return MainTable[position];
		}
	};
}

#endif
