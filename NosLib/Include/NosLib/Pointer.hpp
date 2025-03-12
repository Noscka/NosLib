#ifndef _POINTERS_NOSLIB_HPP_
#define _POINTERS_NOSLIB_HPP_

#include "TypeTraits.hpp"
#include <memory>

namespace NosLib
{
	/// <summary>
	/// namespace which containts Pointer related Items
	/// </summary>
	namespace Pointer
	{
		template <typename Derived, typename Base>
		std::unique_ptr<Derived> DynamicUniquePtrCast(std::unique_ptr<Base>&& basePtr)
		{
			if (Derived* derivedPtr = dynamic_cast<Derived*>(basePtr.get()))
			{
				// Release ownership from the base pointer, so it won't be deleted
				basePtr.release();
				return std::unique_ptr<Derived>(derivedPtr);
			}
			// Cast failed; return an empty unique_ptr.
			return std::unique_ptr<Derived>(nullptr);
		}

		/// <summary>
		/// Finds the pointer to the root value
		/// </summary>
		/// <typeparam name="T">- Pointer Type</typeparam>
		/// <param name="value">- the variable to get searched</param>
		/// <returns>root pointer</returns>
		template<typename T>
		inline constexpr std::add_pointer_t<NosLib::TypeTraits::remove_all_pointers_t<T>> OneOffRootPointer(const T& value)
		{
			if constexpr (std::is_pointer<std::remove_pointer_t<T>>::value)
			{
				return OneOffRootPointer<std::remove_pointer_t<T>>(*value);
			}
			else
			{
				return value;
			}
		}

		/// <summary>
		/// Finds the value at the root of a pointer path
		/// </summary>
		/// <typeparam name="T">- Pointer Type</typeparam>
		/// <param name="value">- the variable to get searched</param>
		/// <returns>root value</returns>
		template<typename T>
		inline constexpr NosLib::TypeTraits::remove_all_pointers_t<T> PointerRootValue(const T& value)
		{
			if constexpr (std::is_pointer<T>::value)
			{
				return PointerRootValue<std::remove_pointer_t<T>>(*value);
			}
			else
			{
				return value;
			}
		}
	}
}

#endif