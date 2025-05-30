#include "ThreadPool.h"

ThreadPool::ThreadPool(size_t numThreads) : stop(false)
{
	for (size_t i = 0; i < numThreads; ++i)
	{
		workers.emplace_back([this] {
			while (true)
			{
				std::function<void()> task;
				{
					std::unique_lock<std::mutex> lock(mtx);
					cv.wait(lock, [this] { return stop || !tasks.empty(); });
					if (stop && tasks.empty())
						return;
					task = std::move(tasks.front());
					tasks.pop();
				}
				task();
			}
			});
	}
}

ThreadPool::~ThreadPool() noexcept
{
	{
		std::unique_lock<std::mutex> lock(mtx);
		stop = true;
	}

	cv.notify_all();

	for (std::thread& worker : workers)
	{
		if (worker.joinable())
		{
			worker.join();
		}
	}
}

void ThreadPool::Enqueue(std::function<void()> func)
{
	{
		std::lock_guard<std::mutex> lock(mtx);
		tasks.push(std::move(func));
	}
	cv.notify_one();
}