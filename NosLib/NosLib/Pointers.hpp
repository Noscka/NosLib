#ifndef _POINTERS_NosLib_HPP_
#define _POINTERS_NosLib_HPP_

#include "TypeTraits.hpp"

namespace NosLib
{
    /// <summary>
    /// namespace which containts Pointer related Items
    /// </summary>
    namespace Pointers
    {
        /// <summary>
        /// Finds the pointer to the root value
        /// </summary>
        /// <typeparam name="T">- Pointer Type</typeparam>
        /// <param name="value">- the variable to get searched</param>
        /// <returns>root pointer</returns>
        template<typename T>
        std::add_pointer_t<NosLib::TypeTraits::remove_all_pointers_t<T>> OneOffRootPointer(const T& value)
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
        NosLib::TypeTraits::remove_all_pointers_t<T> PointerRootValue(const T& value)
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