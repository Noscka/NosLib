#ifndef _FUNCTIONAL_NOSLIB_HPP_
#define _FUNCTIONAL_NOSLIB_HPP_

#include <tuple>
#include <stdexcept>
#include <type_traits>

namespace NosLib
{
	/// <summary>
	/// the base which allows for storing the main class be stored without needing to the define the template
	/// </summary>
	class FunctionStoreBase
	{
	public:
		/// <summary>
		/// Function only defined to allow for outside classes to run the child function
		/// </summary>
		virtual inline void RunFunction() const
		{
			throw std::logic_error("Unimplemented Abstract Class");
		}
	};

	template<class FuncType, typename ... VariadicArgs>
	class FunctionStore : public FunctionStoreBase
	{
	private:
		FuncType* FuncPointer;              /* Pointer to the function */
		std::tuple<VariadicArgs...> Args;   /* a tuple used to store the argument values */
		bool PresetArguments;               /* if the arguments are preset (in the tuple) */
	public:
		inline constexpr FunctionStore(FuncType* funcPointer, VariadicArgs&& ... args)
		{
			FuncPointer = funcPointer;
			Args = std::tuple<VariadicArgs...>(std::forward<VariadicArgs>(args)...);
			PresetArguments = true;
		}

		template<typename = std::enable_if_t<(sizeof...(VariadicArgs) > 0)>>/* only enabled if there are more then 0 variadic arguments */
		inline constexpr FunctionStore(FuncType* funcPointer)
		{
			FuncPointer = funcPointer;
			PresetArguments = false;
		}

		inline void RunFunction() const override
		{
			if (!PresetArguments) /* if trying to use this function without preset variables, throw exception | TODO: convert to compile error */
			{
				throw std::logic_error("Cannot run functional without parameters if the arguments are not preset");
				return;
			}

			std::apply(FuncPointer, Args);
		}

		template<typename = std::enable_if_t<(sizeof...(VariadicArgs) > 0)>> /* only enabled if there are more then 0 variadic arguments */
		inline constexpr void RunFunction(VariadicArgs&& ... args) const
		{
			(*FuncPointer)(std::forward<VariadicArgs>(args)...);
		}

		/// <summary>
		/// if class has preset arguments for the function
		/// </summary>
		/// <returns>if class has preset arguments for the function</returns>
		inline constexpr bool HasPresetArguements() const
		{
			return PresetArguments;
		}
	};

	template<class ObjectType, class FuncType, typename ... VariadicArgs>
	class MemberFunctionStore : public FunctionStoreBase
	{
	private:
		ObjectType* ObjectPointer;			/* Pointer to the object that contains the function */
		FuncType FuncPointer;              /* Pointer to the function */
		std::tuple<VariadicArgs...> Args;   /* a tuple used to store the argument values */
		bool PresetArguments;               /* if the arguments are preset (in the tuple) */
	public:
		inline constexpr MemberFunctionStore(ObjectType* objectPointer, FuncType funcPointer, VariadicArgs&& ... args)
		{
			ObjectPointer = objectPointer;
			FuncPointer = funcPointer;
			Args = std::tuple<VariadicArgs...>(std::forward<VariadicArgs>(args)...);
			PresetArguments = true;
		}

		//template<typename = std::enable_if_t<(sizeof...(VariadicArgs) > 0)>>/* only enabled if there are more then 0 variadic arguments */
		//inline constexpr MemberFunctionStore(ObjectType* objectPointer, FuncType* funcPointer)
		//{
		//	ObjectPointer = objectPointer;
		//	FuncPointer = funcPointer;
		//	PresetArguments = false;
		//}

		inline void RunFunction() const override
		{
			if (!PresetArguments) /* if trying to use this function without preset variables, throw exception | TODO: convert to compile error */
			{
				throw std::logic_error("Cannot run functional without parameters if the arguments are not preset");
				return;
			}

			(ObjectPointer->*FuncPointer)(std::get<VariadicArgs>(Args)...);
		}

		template<typename = std::enable_if_t<(sizeof...(VariadicArgs) > 0)>> /* only enabled if there are more then 0 variadic arguments */
		inline constexpr void RunFunction(VariadicArgs&& ... args) const
		{
			(ObjectPointer->*FuncPointer)(std::forward<VariadicArgs>(args)...);
		}

		/// <summary>
		/// if class has preset arguments for the function
		/// </summary>
		/// <returns>if class has preset arguments for the function</returns>
		inline constexpr bool HasPresetArguements() const
		{
			return PresetArguments;
		}
	};
}

#endif