#include <iostream>
#include <mutex>
#include <condition_variable>

#include "thread_pool.h"
#include "buffer.h"
#include "util.h"

ThreadPool::ThreadPool(int num_threads) : num_threads(num_threads)
{
	buffers = new Buffer[num_threads];
	EXITIFTRUE(buffers == NULL, "allocate buffers fail");
	
	
	
}

ThreadPool::~ThreadPool()
{
	delete[] buffers;
}

bool ThreadPool::are_there_free_buffers()
{
	bool used = false;
	
	for (int i = 0; i < num_threads; i++)
	{
		used = buffers[i].is_used();
		if (used == false)
			return i;
	}
	
	return -1;
}

void ThreadPool::start_job(int low, int high)
{
	std::cout << "ThreadPool: job started l:"
			  << low << " h:" << high << std::endl;
	
	std::unique_lock<std::mutex> lock(mtx);
	while(-1 == are_there_free_buffers())
		all_buffers_captured.wait(lock);
	
}
