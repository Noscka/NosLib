#ifndef _FUNCTIONAL_NOSSTDLIB_HPP_
#define _FUNCTIONAL_NOSSTDLIB_HPP_

namespace NosStdLib
{
	/// <summary>
	/// namespace which contains classes which allow for storing any function and its arguments
	/// </summary>
	namespace Functional
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
            virtual void RunFunction(){ /* Does nothing this class */ }
        };

        /// <summary>
        /// the class which actually stores the function and the parameters
        /// </summary>
        /// <typeparam name="FuncType">- Function return type</typeparam>
        /// <typeparam name="...VariadicArgs">- parameter types</typeparam>
        template<class FuncType, typename ... VariadicArgs>
        class FunctionStore : public FunctionStoreBase
        {
        private:
            FuncType* FuncPointer;              /* Pointer to the function */
            std::tuple<VariadicArgs...> Args;   /* a tuple used to store the argument values */
            bool PresetArguments;               /* if the arguments are preset (in the tuple) */
        public:
            /// <summary>
            /// constructor of the class
            /// </summary>
            /// <param name="funcPointer">- a pointer to the wanted function</param>
            /// <param name="...args">- the arguments</param>
            FunctionStore(FuncType* funcPointer, VariadicArgs&& ... args)
            {
                FuncPointer = funcPointer;
                Args = std::tuple<VariadicArgs...>(std::forward<VariadicArgs>(args)...);
                PresetArguments = true;
            }

			/// <summary>
			/// constructor of the class
			/// </summary>
			/// <param name="funcPointer">- a pointer to the wanted function</param>
			/// <param name="...args">- the arguments</param>
			FunctionStore(FuncType* funcPointer)
			{
				FuncPointer = funcPointer;
                PresetArguments = false;
			}

            /// <summary>
            /// Runs the functions and passes through its arguments
            /// </summary>
            void RunFunction()
            {
                if (!PresetArguments) /* if trying to use this function without preset variables, throw exception | TODO: convert to compile error */
                {
                    throw std::logic_error("Cannot run functional without parameters if the arguments are not preset");
                    return;
                }
                
                std::apply(FuncPointer, Args);
            }

            /// <summary>
            /// Runs the functions with custom arguments
            /// </summary>
            /// <param name="...args">- the custom arguments</param>
            void RunFunction(VariadicArgs&& ... args)
            {
                (*FuncPointer)(std::forward<VariadicArgs>(args)...);
            }

            /// <summary>
            /// if class has preset arguments for the function
            /// </summary>
            /// <returns>if class has preset arguments for the function</returns>
            bool HasPresetArguements()
            {
                return PresetArguments;
            }
        };
	}
}

#endif