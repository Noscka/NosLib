#include <NosLib/Console.hpp>

#include <iostream>
#include <atomic>

int main()
{
	printf("Some Stuff\n");

	printf("Press any button to continue"); getchar();
	NosLib::ClearScreen();

	printf("Other Stuff\n");

	printf("Press any button to continue"); getchar();
	return 0;
}