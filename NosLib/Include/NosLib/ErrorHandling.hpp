#ifndef _ERRORHANDLING_NOSLIB_HPP_
#define _ERRORHANDLING_NOSLIB_HPP_

#include <NosLib/Logging.hpp>
#include <NosLib/ErrorCodes.hpp>

/**
* @brief if statement is true, runs directive and leaves log
*
* Full name:  NOSLOG_ASSERT
* Access:     public
*
* @param[in]  statement   The statement that returns a boolean used to check whether to run the directive
* @param[in]  statementDirective   The directive to be run if statement is true (e.g, return, break, continue, etc)
* @param[in]  logSeverity   Using NosLib::Logging::Severity for log severity
* @param[in]  logMsg   The main message used in log (works the same as std::format)
* @param[in]  ...   variadic parameters to be used in the message above
* @return	  Whatever the statement directive describes
*/
#define NOSLOG_ASSERT(statement, statementDirective, logSeverity, logMsg, ...)\
	do                            \
	{                             \
		if ((statement))          \
		{                         \
			NosLib::Logging::CreateLog(    \
				logSeverity,      \
				logMsg,           \
				 __VA_ARGS__      \
			);                    \
			statementDirective;   \
		}                         \
	} while (0)

/**
* @brief if statement is true, runs directive and leaves log
*
* Full name:  NOS_ASSERT
* Access:     public
*
* @param[in]  statement   The statement that returns a boolean used to check whether to run the directive
* @param[in]  statementDirective   The directive to be run if statement is true (e.g, return, break, continue, etc)
* @return	  Whatever the statement directive describes
*/
#define NOS_ASSERT(statement, statementDirective)\
	do                            \
	{                             \
		if ((statement))          \
		{                         \
			statementDirective;   \
		}                         \
	} while (0)

namespace NosLib
{
	class ResultBase
	{
	protected:
		std::error_code ErrorCodeInternal;

	public:
		ResultBase() = default;
		explicit ResultBase(const std::error_code& errorCode)
			: ErrorCodeInternal(errorCode)
		{}

		virtual ~ResultBase() = default;

		int ErrorValue() const
		{
			return ErrorCodeInternal.value();
		}

		std::string ErrorMessage() const
		{
			return ErrorCodeInternal.message();
		}

		const std::error_category& ErrorCategory() const
		{
			return ErrorCodeInternal.category();
		}

		std::string ErrorCategoryName() const
		{
			return ErrorCategory().name();
		}

		explicit operator bool() const
		{
			return static_cast<bool>(ErrorCodeInternal);
		}
	};

	template<typename ReturnValue>
	class Result : public ResultBase
	{
	private:
		ReturnValue StoredObject;

	public:
		Result() = default;
		using ResultBase::ResultBase;
		inline Result(const ReturnValue& storedObject,
					  const std::error_code& errorCode) :
			StoredObject(storedObject),
			ErrorCodeInternal(errorCode)
		{}

		virtual ~Result() = default;

		inline ReturnValue& GetReturn()
		{
			return StoredObject;
		}

		operator ReturnValue& ()
		{
			return StoredObject;
		}
	};

	template<>
	class Result<void> : public ResultBase
	{
	public:
		Result() = default;
		using ResultBase::ResultBase;

		virtual ~Result() = default;
	};

	#ifdef _WIN32
	std::string GetLastErrorAsString();
	#endif // _WIN32
}

#endif