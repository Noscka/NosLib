#ifndef _LOGGING_NOSLIB_HPP_
#define _LOGGING_NOSLIB_HPP_

#include "DynamicArray.hpp"
#include "String.hpp"

#include <fstream>
#include <chrono>
#include <string>
#include <format>

namespace NosLib
{
	class Logging
	{
	public:
		enum class Severity : uint16_t
		{
			Debug,		/* useful for debug */
			Info,		/* just information */
			Warning,	/* something that could cause problems later but isn't right now */
			Error,		/* a non fatal error, something went wrong but the program can continue */
			Fatal		/* a fatal error, the program has to close because of it */
		};

		/* Will display chosen level and above */
		enum class Verbose : uint16_t 
		{
			Debug,
			Info,
			Warning,
			Error,
			Fatal,
			None
		};
	protected:
		static inline NosLib::DynamicArray<Logging*> Logs;
		static inline Verbose VerboseLevel = Verbose::Warning;

		std::wstring LogMessage;
		Severity LogSeverity;
		std::chrono::system_clock::time_point LogTimestamp;

		constexpr Logging() {}
		Logging(const std::wstring& logMessage, const Severity& logSeverity);

		static constexpr std::wstring SeverityToWstring(const Severity& logSeverity);

	public:
		static constexpr void SetVerboseLevel(const Verbose& verboseLevel);
		static Verbose GetVerboseLevel();

		template<typename CharType>
		static constexpr inline Logging* CreateLog(const std::basic_string<CharType>& logMessage, const Severity& logSeverity)
		{
			Logging* logObject = new Logging(NosLib::String::ConvertString<wchar_t, CharType>(logMessage), logSeverity);
			Logs.Append(logObject);

			/* if severity is lower then Verbose, then don't print or add to file */
			if ((uint16_t)logSeverity < (uint16_t)VerboseLevel)
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

		std::wstring GetLog() const;
	};
}

#endif /* _LOGGING_NOSLIB_HPP_ */
