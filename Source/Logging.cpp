#include <NosLib/Logging.hpp>

using NosLog = NosLib::Logging;

NosLog::Logging(const std::wstring& logMessage, const NosLog::Severity& logSeverity)
{
	LogMessage = logMessage + (logMessage.back() != L'\n' ? L"\n" : L"");
	LogSeverity = logSeverity;
	LogTimestamp = std::chrono::system_clock::now();
}

std::wstring NosLog::SeverityToWstring(const NosLog::Severity& logSeverity)
{
	switch (logSeverity)
	{
	case NosLog::Severity::Debug:
		return L"Debug";
		break;
	case NosLog::Severity::Info:
		return L"Info";
		break;
	case NosLog::Severity::Warning:
		return L"Warning";
		break;
	case NosLog::Severity::Error:
		return L"Error";
		break;
	case NosLog::Severity::Fatal:
		return L"Fatal";
		break;
	}
	return L"UNKNOWN";
}

void NosLog::SetVerboseLevel(const Verbose& verboseLevel)
{
	VerboseLevel = verboseLevel;
}

NosLog::Verbose NosLog::GetVerboseLevel()
{
	return VerboseLevel;
}

std::wstring NosLog::GetLog() const
{
	// %d/%m/%Y for date too
	return std::format(L"({}) {:%X} {}", SeverityToWstring(LogSeverity), std::chrono::zoned_time(std::chrono::current_zone(), LogTimestamp), LogMessage);
}