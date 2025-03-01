#include <NosLib/Logging.hpp>

#include <format>

using NosLog = NosLib::Logging;

NosLog::Logging(const NosString& logMessage, const NosLog::Severity& logSeverity)
{
	LogMessage = logMessage;
	if (LogMessage.back() != static_cast<NosChar>(L'\n'))
	{
		LogMessage +=  (const NosChar*)(L"\n");
	}

	LogSeverity = logSeverity;
	LogTimestamp = std::chrono::system_clock::now();
}

NosLib::NosString NosLog::SeverityToString(const NosLog::Severity& logSeverity)
{
	switch (logSeverity)
	{
	case NosLog::Severity::Debug:
		return "Debug";

	case NosLog::Severity::Info:
		return "Info";

	case NosLog::Severity::Warning:
		return "Warning";

	case NosLog::Severity::Error:
		return "Error";

	case NosLog::Severity::Fatal:
		return "Fatal";
	}

	return "UNKNOWN";
}

void NosLog::SetVerboseLevel(const Verbose& verboseLevel)
{
	VerboseLevel = verboseLevel;
}

NosLog::Verbose NosLog::GetVerboseLevel()
{
	return VerboseLevel;
}

NosLib::NosString NosLog::GetLog() const
{
	constexpr NosString_View formatBody("({}) {:%X} {}");
	NosString outLog = std::format(formatBody, SeverityToString(LogSeverity), std::chrono::zoned_time(std::chrono::current_zone(), LogTimestamp), LogMessage);

	// %d/%m/%Y for date too
	return outLog;
}