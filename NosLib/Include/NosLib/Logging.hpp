#ifndef _LOGGING_NOSLIB_HPP_
#define _LOGGING_NOSLIB_HPP_

#include <NosLib/Internal/Export.hpp>
#include <NosLib/String.hpp>
#include <NosLib/RGB.hpp>
#include <NosLib/ErrorHandling.hpp>

#include <chrono>
#include <cstdint>

namespace NosLib
{
	class NOSLIB_API Logging
	{
	public:
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

		std::string LogMessage;
		Severity LogSeverity{};
		std::chrono::system_clock::time_point LogTimestamp;

		Logging() = default;
		Logging(const std::string&, const Severity&);

		static std::string SeverityToString(const Severity&);
		static NosRGB SeverityToColor(const Severity&);
		static std::string& GenerateLogFormat();
		void WriteToFile(const std::string&) const;
		void PrintLog() const;

	public:
		static void SetVerboseLevel(const Verbose&);
		static Verbose GetVerboseLevel();

		std::string GetLog() const;

		template<typename... fmtArgs>
		static inline void CreateLog(const Severity& logSeverity, const std::string& logMessage, fmtArgs... formatArgs)
		{
			std::string formattedLog = std::vformat(logMessage, std::make_format_args(formatArgs...));
			Logging logObject(formattedLog, logSeverity);
			logObject.WriteToFile("full-log.txt");

			/* if severity is lower then Verbose, then don't print or add to file */
			NOS_ASSERT(static_cast<uint8_t>(logSeverity) < static_cast<uint8_t>(VerboseLevel), return);

			logObject.PrintLog();
			logObject.WriteToFile("log.txt");
		}
	};
}

#endif /* _LOGGING_NOSLIB_HPP_ */
