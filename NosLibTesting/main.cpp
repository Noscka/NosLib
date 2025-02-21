#include <NosLib/Logging.hpp>

#include <iostream>
#include <atomic>

int main()
{
	using NosLog = NosLib::Logging;

	NosLog* log = NosLog::CreateLog<char>("ABC", NosLib::Logging::Severity::Info);
	
	printf("%s\n", NosLib::String::ToString(log->GetLog()).c_str());

	/*NosLib::DynamicArray<int> customArray({1,2,3,4});

	for (int entry : customArray)
	{
		printf("%d\n", entry);
	}*/

	printf("Press any button to continue"); getchar();
	return 0;
}