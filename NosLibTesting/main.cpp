#include <NosLib/Logging.hpp>

#include <iostream>
#include <atomic>

int main()
{
	using NosLog = NosLib::Logging;

	NosLog::Ptr log = NosLog::CreateLog(NosLib::Logging::Severity::Info, "ABC {}", "Some String");
	
	printf("%s\n", log->GetLog().c_str());

	/*NosLib::DynamicArray<int> customArray({1,2,3,4});

	for (int entry : customArray)
	{
		printf("%d\n", entry);
	}*/

	printf("Press any button to continue"); getchar();
	return 0;
}