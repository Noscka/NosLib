#include <NosLib/ThreadPool.hpp>

#include <iostream>

#include <conio.h>

#include <NosLib/HttpClient.hpp>

std::mutex printingMutex;

void PrintSomething(const int& number)
{
	//std::lock_guard<std::mutex> lock(printingMutex);
	printf("printed Something %d\n", number);
	std::this_thread::sleep_for(std::chrono::seconds(10));
}

int main()
{
	NosLib::HostPath github("https://github.com/Noscka/Norzkas-Custom-Gamma-Installer/archive/refs/heads/master.zip");

	std::unique_ptr<NosLib::HttpClient> testClient = NosLib::HttpClient::MakeClient(NosLib::String::ToString(github.Host));
	testClient->set_keep_alive(true);
	testClient->set_follow_location(true);
	//testClient->DownloadFile(NosLib::String::ToString(github.Path), "here.zip");

	return 0;

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
}\