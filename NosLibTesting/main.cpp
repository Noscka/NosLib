#include <NosLib/ThreadPool.hpp>

#include <iostream>
#include <conio.h>

class TestClass
{
private:
	int QuickStore = -1;
public:
	TestClass() = default;
	TestClass(const int& quickStore)
	{
		QuickStore = quickStore;
	}

	void PrintFunction(const int& anotherNumber)
	{
		printf("%d | %d\n", QuickStore, anotherNumber);
		QuickStore = anotherNumber+1;
		printf("%d | %d\n", QuickStore, anotherNumber);
	}
};

int main()
{
	NosLib::ThreadPool threadPool;

	TestClass someClass1;

	NosLib::MemberFunctionStore testFunc1(&someClass1, &TestClass::PrintFunction, 2);

	printf("Thread pool Created\n");
	threadPool.StartThreadPool(testFunc1, true);

	printf("Thread Pool Started\n");

	threadPool.JoinThreadPool();
	printf("Thread Pool finished and joined\n");

	printf("Press any button to continue"); _getch();
	return 0;
}