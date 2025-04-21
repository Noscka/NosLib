#include <NosLib/Http/URL.hpp>

int main()
{
	NosLib::URL testURL("https://DickSuckFor5Bucks.com");

	printf("Scheme: %s | Domain: %s | Subdir: %s\n",
		   testURL.GetScheme().c_str(),
		   testURL.GetDomain().c_str(),
		   testURL.GetSubdir().c_str());

	printf("Press any button to continue"); getchar();
	return 0;
}