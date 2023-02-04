#ifndef _TYPETRAITS_NOSSTDLIB_HPP_
#define _TYPETRAITS_NOSSTDLIB_HPP_

#include <type_traits>

namespace NosStdLib
{
	/// <summary>
	/// namespace which containts TypeTrait
	/// </summary>
	namespace TypeTraits
	{
		/// <summary>
		/// Goes to core type
		/// </summary>
		/// <typeparam name="T">- type to remove pointers from</typeparam>
		template<typename T>
		struct remove_all_pointers : std::conditional_t<
			std::is_pointer_v<T>,
			remove_all_pointers<
			std::remove_pointer_t<T>
			>,
			std::type_identity<T>
		>
		{};

		/// <summary>
		/// Goes to core type
		/// </summary>
		/// <typeparam name="T">- type to remove pointers from</typeparam>
		template<typename T>
		using remove_all_pointers_t = typename remove_all_pointers<T>::type;

	}
}

#endif