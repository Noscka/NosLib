#ifndef _CAST_NosLib_HPP_
#define _CAST_NosLib_HPP_

#include <type_traits>

namespace NosLib
{
	/// <summary>
	/// casts TypeFrom to TypeTo specified in template
	/// </summary>
	/// <typeparam name="TypeFrom">- type to convert from</typeparam>
	/// <typeparam name="TypeTo">- type to convert to</typeparam>
	/// <param name="typeIn">- the actual data getting casted</param>
	/// <returns>casted data</returns>
	template<typename TypeTo, typename TypeFrom>
	inline constexpr TypeTo Cast(const TypeFrom& typeIn)
	{
		if constexpr (std::is_same_v<TypeTo, TypeFrom>)
		{
			return typeIn;
		}
		else
		{
			return (TypeTo)typeIn;
		}
	}
}

#endif