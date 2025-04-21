#include <NosLib/Logging.hpp>

int main()
{
	std::string badStr = "{Yo wassup} {";
	std::string goodStr = NosLib::Logging::SanitizeString(badStr);

	printf("bad Str: %s\n", badStr.c_str());
	printf("good Str: %s\n", goodStr.c_str());

	/*NosLib::URL testURL("https://DickSuckFor5Bucks.com");

	printf("Scheme: %s | Domain: %s | Subdir: %s\n",
		   testURL.GetScheme().c_str(),
		   testURL.GetDomain().c_str(),
		   testURL.GetSubdir().c_str());*/

	printf("Press any button to continue"); getchar();
	return 0;
}