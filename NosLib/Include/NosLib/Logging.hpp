#ifndef _LOGGING_NOSLIB_HPP_
#define _LOGGING_NOSLIB_HPP_

#include "Internal/String.hpp"
#include "Internal/Export.hpp"
#include "DynamicArray.hpp"
#include "String.hpp"

#include <memory>
#include <fstream>
#include <chrono>
#include <string>
#include <cstdint>

namespace NosLib
{
	class NOSLIB_API Logging
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
		static Verbose VerboseLevel;

		NosString LogMessage;
		Severity LogSeverity{};
		std::chrono::system_clock::time_point LogTimestamp;

		Logging() = default;
		Logging(const NosString&, const Severity&);

		static NosString SeverityToString(const Severity&);
		static NosString& GenerateLogFormat();
		void WriteToFile(const std::string&) const;

	public:
		static void SetVerboseLevel(const Verbose&);
		static Verbose GetVerboseLevel();

		NosString GetLog() const;

		template<typename CharType, typename... fmtArgs>
		static inline Ptr CreateLog(const Severity& logSeverity, const std::basic_string<CharType>& logMessage, fmtArgs... formatArgs)
		{
			using InString = std::basic_string<CharType>;

			InString formattedLog = std::vformat(logMessage, std::make_format_args(formatArgs...));
			NosString formattedNosLog = String::ConvertString<NosChar, CharType>(formattedLog);
			Ptr logObject(new Logging(formattedNosLog, logSeverity));

			logObject->WriteToFile("full-log.txt");
			/* if severity is lower then Verbose, then don't print or add to file */
			if (static_cast<uint8_t>(logSeverity) < static_cast<uint8_t>(VerboseLevel))
			{
				return logObject;
			}

			NosString containedLogMessage = logObject->GetLog();
			printf(NosLib::String::ConvertString<char, NosChar>(containedLogMessage).c_str());

			logObject->WriteToFile("log.txt");

			return logObject;
		}

		template<typename CharType, typename... fmtArgs>
		static inline Ptr CreateLog(const Severity& logSeverity, const CharType* logMessage, fmtArgs... formatArgs)
		{
			return CreateLog(logSeverity, std::basic_string<CharType>(logMessage), std::forward<fmtArgs>(formatArgs)...);
		}
	};
}

#endif /* _LOGGING_NOSLIB_HPP_ */
