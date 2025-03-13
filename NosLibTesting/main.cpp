#include <NosLib/Hash.hpp>

#include <iostream>
#include <atomic>

int main()
{
	NosLib::GetHash("NosLibTesting.exe", EVP_sha256());

	printf("Press any button to continue"); getchar();
	return 0;
}