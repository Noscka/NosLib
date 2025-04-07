#include <NosLib/Logging.hpp>

#include <format>

using NosLog = NosLib::Logging;

NosLog::Verbose NosLog::VerboseLevel = NosLog::Verbose::Warning;

NosLog::Logging(const std::string& logMessage, const NosLog::Severity& logSeverity)
{
	LogMessage = logMessage;
	if (LogMessage.back() != static_cast<NosChar>(L'\n'))
	{
		LogMessage += (const NosChar*)(L"\n");
	}

	LogSeverity = logSeverity;
	LogTimestamp = std::chrono::system_clock::now();
}

std::string NosLog::SeverityToString(const NosLog::Severity& logSeverity)
{
	switch (logSeverity)
	{
	case Severity::Debug:
		return "Debug";

	case Severity::Info:
		return "Info";

	case Severity::Warning:
		return "Warning";

	case Severity::Error:
		return "Error";

	case Severity::Fatal:
		return "Fatal";
	}

	return "UNKNOWN";
}

std::string& NosLog::GenerateLogFormat()
{
	static std::string LogFormat;

	if (!LogFormat.empty())
	{
		return LogFormat;
	}

	size_t longestSeverityString = 0;

	for (uint8_t i = 0; i <= (uint8_t)Severity::Fatal; i++)
	{
		size_t currentSeveritySize = SeverityToString((Severity)i).size();

		if (currentSeveritySize > longestSeverityString)
		{
			longestSeverityString = currentSeveritySize;
		}
	}

	longestSeverityString += 2; /* 2 because of brackets */

	LogFormat = std::format("{{:<{}}} {{:<9}} {{}}", longestSeverityString);

	return LogFormat;
}

void NosLog::WriteToFile(const std::string& fileName) const
{
	std::string containedLogMessage = GetLog();

	std::ofstream outLog(fileName, std::ios::binary | std::ios::app);
	outLog.write(containedLogMessage.c_str(), containedLogMessage.size());
	outLog.close();
}

void NosLog::SetVerboseLevel(const Verbose& verboseLevel)
{
	VerboseLevel = verboseLevel;
}

NosLog::Verbose NosLog::GetVerboseLevel()
{
	return VerboseLevel;
}

std::string NosLog::GetLog() const
{
	std::string severityString = std::format("({})", SeverityToString(LogSeverity));
	std::string chronoTimestamp = std::format("{:%X}", std::chrono::zoned_time(std::chrono::current_zone(), LogTimestamp));

	// %d/%m/%Y for date too
	std::string outLog = std::vformat(GenerateLogFormat(),
											std::make_format_args(
												severityString,
												chronoTimestamp,
												LogMessage));

	return outLog;
}