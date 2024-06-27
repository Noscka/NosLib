#include <NosLib/ThreadPool.hpp>

#include <iostream>

#include <conio.h>

std::mutex printingMutex;

void PrintSomething(const int& number)
{
	//std::lock_guard<std::mutex> lock(printingMutex);
	printf("printed Something %d\n", number);
	std::this_thread::sleep_for(std::chrono::seconds(10));
}

int main()
{
	auto* threadPool = NosLib::ThreadPool::CreateThreadPool();

	NosLib::FunctionStore printFunction(&PrintSomething, 1);
	printf("Thread pool Created\n");
	threadPool->StartThreadPool(printFunction, true);

	printf("Thread Pool Started\n");

	threadPool->JoinThreadPool();
	printf("Thread Pool finished and joined\n");

	delete threadPool;

	printf("Press any button to continue"); _getch();
	return 0;
}