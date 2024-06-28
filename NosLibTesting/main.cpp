#include <NosLib/ThreadPool.hpp>

#include <iostream>
#include <atomic>
#include <conio.h>

class TestClass
{
private:
	static inline std::atomic<int> GlobalIndex = 0;
	int CurrentIndex;
public:
	TestClass()
	{
		CurrentIndex = GlobalIndex++;
	}

	void PrintFunction(const int& anotherNumber)
	{
		printf("index: %d | %d\n", CurrentIndex, anotherNumber);
	}
};

int main()
{
	NosLib::ThreadPool threadPool;

	NosLib::MemberFunctionStore<TestClass, void(TestClass::*)(const int&), int> testFunc1(nullptr, &TestClass::PrintFunction, 2);

	printf("Thread pool Created\n");
	threadPool.StartThreadPool(testFunc1, true, 2.0f);

	printf("Thread Pool Started\n");

	threadPool.JoinThreadPool();
	printf("Thread Pool finished and joined\n");

	printf("Press any button to continue"); _getch();
	return 0;
}