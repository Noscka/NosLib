#ifndef _THREADPOOL_NOSLIB_HPP_
#define _THREADPOOL_NOSLIB_HPP_

#include "DynamicArray.hpp"
#include "Logging.hpp"
#include "Functional.hpp"

#include <mutex>
#include <condition_variable>
#include <thread>
#include <cstdint>

namespace NosLib
{
	class ThreadPool
	{
	protected:
		NosLib::DynamicArray<std::thread*> ThreadPoolArray;
		NosLib::FunctionStoreBase* ThreadFunction;

		std::mutex threadJoinMutex;
		std::condition_variable threadJoinCV;

		bool StopSignal = false;

		constexpr static unsigned int DefaultCoreCount = 8;
		unsigned int CustomThreadCount = 0;

		inline void ManageThreads()
		{
			for (int i = 0; i <= ThreadPoolArray.GetLastArrayIndex(); /* ArrayIndex will go down */)
			{
				ThreadPoolArray[i]->join();
				ThreadPoolArray.Remove(i);
				NosLib::Logging::CreateLog<char>(std::format("Thread {} finished", i), NosLib::Logging::Severity::Debug);
			}

			delete ThreadFunction;

			NosLib::Logging::CreateLog<char>("Thread Pool finished work", NosLib::Logging::Severity::Debug);
		}

		inline void ThreadPoolManagement()
		{
			std::lock_guard<std::mutex> lk(threadJoinMutex);

			unsigned int threadCount = GetAmountOfCores();

			for (unsigned int i = 0; i < threadCount; i++)
			{
				ThreadPoolArray.Append(new std::thread([this]() { ThreadFunction->RunFunction(); }));
				NosLib::Logging::CreateLog<char>(std::format("Thread {} started", i), NosLib::Logging::Severity::Debug);
				//Sleep(1); /* desync threads */
			}

			ManageThreads();
			threadJoinCV.notify_all();
			/* in theory, thread deletes itself here */
		}

	public:
		inline static ThreadPool* CreateThreadPool()
		{
			auto* threadPool = new ThreadPool;
			
			return threadPool;
		}

		template<class FuncType, typename ... VariadicArgs>
		inline void StartThreadPool(const NosLib::FunctionStore<FuncType, VariadicArgs...>& threadFunction, const bool& detachThread = false, const unsigned int& customThreadCount = 0)
		{
			using ThreadFunctionType = NosLib::FunctionStore<FuncType, VariadicArgs...>;

			ThreadFunction = new ThreadFunctionType(threadFunction);
			CustomThreadCount = customThreadCount;

			if (detachThread)
			{
				std::thread thisThread(&ThreadPool::ThreadPoolManagement, this);
				thisThread.detach();
			}
			else
			{
				ThreadPoolManagement();
			}
		}

		/* Will wait for the thread pool to be finished */
		inline void JoinThreadPool()
		{
			std::unique_lock<std::mutex> lk(threadJoinMutex);
			threadJoinCV.wait(lk);
		}

	protected:
		inline unsigned int GetAmountOfCores()
		{
			if (CustomThreadCount != 0)
			{
				return CustomThreadCount;
			}

			unsigned int coreCount = std::thread::hardware_concurrency();

			if (coreCount == 0)
			{
				coreCount = DefaultCoreCount;
			}

			coreCount *= 2;

			return coreCount;
		}
	};
}

#endif