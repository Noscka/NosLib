#include <NosLib/Console.hpp>
#include <NosLib/ANSI.hpp>

#include <iostream>
#include <atomic>

int main()
{
	printf("%s\n", (NosLib::CheckANSI() ? "ANSI On" : "ANSI Off"));
	NosLib::EnableANSI();
	printf("%s\n", (NosLib::CheckANSI() ? "ANSI On" : "ANSI Off"));

	std::string coloredText = NosLib::ColorString("Colored Text", { 255,0,160 });

	printf("%s\n", coloredText.c_str());

	printf("Press any button to continue"); getchar();
	return 0;
}