#pragma once
#include <vector>
#include <thread>
#include <queue>
#include <functional>
#include <mutex>
#include <condition_variable>
#include <atomic>

class ThreadPool
{
private:
	std::vector<std::thread> workers;
	std::queue<std::function<void()>> tasks;
	std::mutex mtx;
	std::condition_variable cv;
	std::atomic<bool> stop;
public:
	ThreadPool(size_t numThreads);
	~ThreadPool() noexcept;
	void Enqueue(std::function<void()> task);
};


