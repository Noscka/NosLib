#include <NosLib/Logging.hpp>
#include <NosLib/ANSI.hpp>
#include <NosLib/Console.hpp>

#include <fstream>

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

NosLib::NosRGB NosLog::SeverityToColor(const NosLog::Severity& logSeverity)
{
	switch (logSeverity)
	{
	case Severity::Debug:
		return { 0, 122, 204 };

	case Severity::Info:
		return { 220, 220, 220 };

	case Severity::Warning:
		return { 255, 204, 0 };

	case Severity::Error:
		return { 255, 0, 0 };

	case Severity::Fatal:
		return { 255, 69, 0 };
	}

	return { 255,255,255 };
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
	outLog.write(containedLogMessage.c_str(), static_cast<std::streamsize>(containedLogMessage.size()));
	outLog.close();
}

void NosLog::PrintLog() const
{
	std::string logString = GetLog();

	if (NosLib::CheckANSI())
	{
		logString = NosLib::ColorString(logString, SeverityToColor(LogSeverity));
	}

	printf("%s", logString.c_str());
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