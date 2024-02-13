#ifndef _TYPETRAITS_NosLib_HPP_
#define _TYPETRAITS_NosLib_HPP_

#include <type_traits>

namespace NosLib
{
	/// <summary>
	/// namespace which containts TypeTrait
	/// </summary>
	namespace TypeTraits
	{
		/// <summary>
		/// Specifies what ::value will return
		/// </summary>
		template<bool ReturnValue>
		struct TypeTraitReturn { static const bool value = ReturnValue; };

	#pragma region remove_all_pointers
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
	#pragma endregion

	#pragma region is_character
		/// <summary>
		/// Checks if datatype is a character datatype
		/// </summary>
		template<typename T>
		struct is_character : TypeTraitReturn<false> {};

		template<>
		struct is_character<char> : TypeTraitReturn<true> {};

		template<>
		struct is_character<wchar_t> : TypeTraitReturn<true> {};

		template<>
		struct is_character<char8_t> : TypeTraitReturn<true> {};

		template<>
		struct is_character<char16_t> : TypeTraitReturn<true> {};

		template<>
		struct is_character<char32_t> : TypeTraitReturn<true> {};

		template<typename T>
		constexpr bool is_character_v = is_character<T>::value;
	#pragma endregion
	}
}
#endif