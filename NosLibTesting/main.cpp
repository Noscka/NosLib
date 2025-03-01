#include <NosLib/Logging.hpp>

#include <iostream>
#include <atomic>

int main()
{
	using NosLog = NosLib::Logging;

	NosLog::SetVerboseLevel(NosLog::Verbose::Debug);

	NosLog::CreateLog(NosLib::Logging::Severity::Debug, "ABC {}", "Some String");
	NosLog::CreateLog(NosLib::Logging::Severity::Info, "ABC {}", "Some String");
	NosLog::CreateLog(NosLib::Logging::Severity::Warning, "ABC {}", "Some String");
	NosLog::CreateLog(NosLib::Logging::Severity::Error, "ABC {}", "Some String");
	NosLog::CreateLog(NosLib::Logging::Severity::Fatal, "ABC {}", "Some String");

	/*NosLib::DynamicArray<int> customArray({1,2,3,4});

	for (int entry : customArray)
	{
		printf("%d\n", entry);
	}*/

	printf("Press any button to continue"); getchar();
	return 0;
}