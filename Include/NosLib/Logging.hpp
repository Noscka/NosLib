#ifndef _LOGGING_NOSLIB_HPP_
#define _LOGGING_NOSLIB_HPP_

#include "DynamicArray.hpp"
#include "String.hpp"

#include <memory>
#include <fstream>
#include <chrono>
#include <string>
#include <format>

namespace NosLib
{
	class Logging
	{
	public:
		using Ptr = std::unique_ptr<Logging>;

		enum class Severity : uint8_t
		{
			Debug,		/* useful for debug */
			Info,		/* just information */
			Warning,	/* something that could cause problems later but isn't right now */
			Error,		/* a non fatal error, something went wrong but the program can continue */
			Fatal		/* a fatal error, the program has to close because of it */
		};

		/* Will display chosen level and above */
		enum class Verbose : uint8_t
		{
			Debug,
			Info,
			Warning,
			Error,
			Fatal,
			None
		};
	protected:
		//static inline NosLib::DynamicArray<Logging*> Logs; /* I don't see why keep logs in memory */
		static inline Verbose VerboseLevel = Verbose::Warning;

		std::wstring LogMessage;
		Severity LogSeverity;
		std::chrono::system_clock::time_point LogTimestamp;

		constexpr Logging() {}
		Logging(const std::wstring& logMessage, const Severity& logSeverity);

		static std::wstring SeverityToWstring(const Severity& logSeverity);

	public:
		static void SetVerboseLevel(const Verbose& verboseLevel);
		static Verbose GetVerboseLevel();

		template<typename CharType, typename... frmArgs>
		static constexpr inline Ptr CreateLog(const Severity& logSeverity, const std::basic_string<CharType>& logMessage, frmArgs... formatArgs)
		{
			using SpecifiedFormatContext = std::basic_format_context<std::back_insert_iterator<std::_Fmt_buffer<CharType>>, CharType>;

			std::basic_string<CharType> formattedLog = std::vformat(logMessage, std::make_format_args<SpecifiedFormatContext>(formatArgs...));
			Ptr logObject(new Logging(NosLib::String::ConvertString<wchar_t, CharType>(formattedLog), logSeverity));
			//Logs.Append(logObject);

			/* if severity is lower then Verbose, then don't print or add to file */
			if (static_cast<uint8_t>(logSeverity) >= static_cast<uint8_t>(VerboseLevel))
			{
				return logObject;
			}

			std::wstring containedLogMessage = logObject->GetLog();
			fprintf(stderr, NosLib::String::ConvertString<char, wchar_t>(containedLogMessage).c_str());

			std::wofstream outLog("log.txt", std::ios::binary | std::ios::app);
			outLog.write(containedLogMessage.c_str(), containedLogMessage.size());
			outLog.close();

			return logObject;
		}

		template<typename CharType, typename... frmArgs>
		static constexpr inline Ptr CreateLog(const Severity& logSeverity, const CharType* logMessage, frmArgs... formatArgs)
		{
			return CreateLog(logSeverity, std::basic_string<CharType>(logMessage), std::forward<frmArgs>(formatArgs)...);
		}

		std::wstring GetLog() const;
	};
}

#endif /* _LOGGING_NOSLIB_HPP_ */
