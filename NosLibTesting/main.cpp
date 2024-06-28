#include <NosLib/Functional.hpp>

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
	}
};

template<class FuncType>
void CallClassFunction(TestClass* objPtr, FuncType funcPtr, const int& param)
{
	(objPtr->*funcPtr)(param);
}

int main()
{
	TestClass someClass1;
	TestClass someClass2(10);

	NosLib::MemberFunctionStore testFunc1(&someClass1, &TestClass::PrintFunction, -1);
	NosLib::MemberFunctionStore testFunc2(&someClass2, &TestClass::PrintFunction, 1);

	printf("Created\n");

	testFunc1.RunFunction();
	testFunc2.RunFunction();

	printf("Press any button to continue"); _getch();
	return 0;
}