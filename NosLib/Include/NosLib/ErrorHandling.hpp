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
#ifdef _WIN32
#define NOSLOG_ASSERT(statement, statementDirective, logSeverity, logMsg, ...)\
	do									\
	{									\
		if ((statement))				\
		{								\
			NosLib::Logging::CreateLog( \
				logSeverity,			\
				logMsg,				    \
				__VA_ARGS__				\
			);							\
			statementDirective;			\
		}								\
	} while (0)
#else
#define NOSLOG_ASSERT(statement, statementDirective, logSeverity, logMsg, ...)\
	do									\
	{									\
		if ((statement))				\
		{								\
			NosLib::Logging::CreateLog( \
				logSeverity,			\
				logMsg __VA_OPT__(, )   \
				__VA_ARGS__				\
			);							\
			statementDirective;			\
		}								\
	} while (0)
#endif // _WIN32

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
		NosString AdditionalErrorMessage;

	public:
		inline ResultBase() noexcept = default;
		inline ResultBase(const std::error_code& errorCode) noexcept :
			ErrorCodeInternal(errorCode)
		{}

		template <class enumType, std::enable_if_t<std::is_error_code_enum_v<enumType>, int> = 0>
		inline ResultBase(enumType errorCode) noexcept :
			ErrorCodeInternal(errorCode)
		{}

		inline ResultBase(const std::error_code& errorCode, const std::string& additionalErrorMessage) noexcept :
			ErrorCodeInternal(errorCode),
			AdditionalErrorMessage(additionalErrorMessage)
		{}

		template <class enumType, std::enable_if_t<std::is_error_code_enum_v<enumType>, int> = 0>
		inline ResultBase(enumType errorCode, const std::string& additionalErrorMessage) noexcept :
			ErrorCodeInternal(errorCode),
			AdditionalErrorMessage(additionalErrorMessage)
		{}

		virtual ~ResultBase() = default;

		const std::error_code& ErrorCode() const noexcept
		{
			return ErrorCodeInternal;
		}

		int ErrorValue() const noexcept
		{
			return ErrorCodeInternal.value();
		}

		std::string ErrorMessage() const noexcept
		{
			return ErrorCodeInternal.message();
		}

		std::string GetAdditionalErrorMessage() const noexcept
		{
			return AdditionalErrorMessage;
		}

		const std::error_category& ErrorCategory() const noexcept
		{
			return ErrorCodeInternal.category();
		}

		std::string ErrorCategoryName() const noexcept
		{
			return ErrorCategory().name();
		}

		explicit operator bool() const noexcept
		{
			/* Reverse since error code returns true if there is an error
			 * I want to have this object return true if the object is valid (no error) */
			return !static_cast<bool>(ErrorCodeInternal);
		}
	};

	template<typename ReturnValue>
	class Result : public ResultBase
	{
	private:
		ReturnValue StoredObject;

	public:
		inline Result() noexcept = default;
		using ResultBase::ResultBase;

		inline Result(const ReturnValue& storedObject) noexcept :
			StoredObject(storedObject)
		{}

		inline Result(const ReturnValue& storedObject,
					  const std::error_code& errorCode) noexcept :
			StoredObject(storedObject),
			ResultBase(errorCode)
		{}

		virtual ~Result() = default;

		inline ReturnValue& GetReturn() noexcept
		{
			return StoredObject;
		}

		template<typename U = ReturnValue,
			typename = std::enable_if_t<!std::is_same_v<U, bool>>>
		explicit operator ReturnValue&() noexcept
		{
			return StoredObject;
		}

		ReturnValue& operator*()
		{
			return StoredObject;
		}
		const ReturnValue& operator*() const
		{
			return StoredObject;
		}

		ReturnValue* operator->()
		{
			return &StoredObject;
		}
		const ReturnValue* operator->() const
		{
			return &StoredObject;
		}
	};

	template<typename ReturnValue>
	class Result<std::unique_ptr<ReturnValue>> : public ResultBase
	{
	private:
		std::unique_ptr<ReturnValue> StoredObject;

	public:
		inline Result() noexcept = default;
		using ResultBase::ResultBase;

		inline Result(std::unique_ptr<ReturnValue>&& storedObject) noexcept :
			StoredObject(std::move(storedObject))
		{}

		inline Result(const std::error_code& errorCode) noexcept :
			ResultBase(errorCode)
		{}

		std::unique_ptr<ReturnValue>& GetReturn() noexcept
		{
			return StoredObject;
		}

		operator std::unique_ptr<ReturnValue>& () noexcept
		{
			return StoredObject;
		}

		/* Move constructor/operator */
		Result(Result&& other) noexcept = default;
		Result& operator=(Result&& other) noexcept = default;

		/* Copy constructor/operator */
		Result(const Result&) = delete;
		Result& operator=(const Result&) = delete;
	};

	template<>
	class NOSLIB_API Result<void> : public ResultBase
	{
	public:
		inline Result() = default;
		using ResultBase::ResultBase;

		virtual ~Result() = default;
	};

	#ifdef _WIN32
	std::string NOSLIB_API GetLastErrorAsString();
	#endif // _WIN32
}

#endif