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
            virtual void RunFunction(){/* Does nothing this class */ }
        };

        /// <summary>
        /// the class which actuallys stores the function and the parameters
        /// </summary>
        /// <typeparam name="FuncType">- Function return type</typeparam>
        /// <typeparam name="...VariadicArgs">- parameter types</typeparam>
        template<class FuncType, typename ... VariadicArgs>
        class FunctionStore : public FunctionStoreBase
        {
        private:
            FuncType* FuncPointer;              /* Pointer to the function */
            std::tuple<VariadicArgs...> Args;   /* a tuple used to store the argument values */
        public:

            /// <summary>
            /// constructer of the class
            /// </summary>
            /// <param name="funcPointer">- a pointer to the wanted function</param>
            /// <param name="...args">- the arguments</param>
            FunctionStore(FuncType* funcPointer, VariadicArgs&& ... args)
            {
                FuncPointer = funcPointer;
                Args = std::tuple<VariadicArgs...>(std::forward<VariadicArgs>(args)...);
            }

            /// <summary>
            /// Runs the functions and passes through its arguments
            /// </summary>
            void RunFunction()
            {
                std::apply(FuncPointer, Args);
            }
        };
	}
}

#endif