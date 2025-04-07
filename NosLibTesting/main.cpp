#include <NosLib/Logging.hpp>
#include <NosLib/Console.hpp>

int main()
{
	using NosLog = NosLib::Logging;

	NosLog::SetVerboseLevel(NosLog::Verbose::Debug);

	NosLog::CreateLog(NosLib::Logging::Severity::Debug, "ABC {}", "Some String");
	NosLog::CreateLog(NosLib::Logging::Severity::Info, "ABC {}", "Some String");
	NosLog::CreateLog(NosLib::Logging::Severity::Warning, "ABC {}", "Some String");
	NosLog::CreateLog(NosLib::Logging::Severity::Error, "ABC {}", "Some String");
	NosLog::CreateLog(NosLib::Logging::Severity::Fatal, "ABC {}", "Some String");

	printf("%s\n", (NosLib::CheckANSI() ? "ANSI On" : "ANSI Off"));
	NosLib::EnableANSI();
	printf("%s\n", (NosLib::CheckANSI() ? "ANSI On" : "ANSI Off"));

	NosLog::CreateLog(NosLib::Logging::Severity::Debug, "ABC {}", "Some String");
	NosLog::CreateLog(NosLib::Logging::Severity::Info, "ABC {}", "Some String");
	NosLog::CreateLog(NosLib::Logging::Severity::Warning, "ABC {}", "Some String");
	NosLog::CreateLog(NosLib::Logging::Severity::Error, "ABC {}", "Some String");
	NosLog::CreateLog(NosLib::Logging::Severity::Fatal, "ABC {}", "Some String");

	printf("Press any button to continue"); getchar();
	return 0;
}