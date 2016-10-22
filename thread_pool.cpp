#include <iostream>

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

void ThreadPool::start_job(int low, int high)
{	
	std::cout << "ThreadPool: job started l:"
			  << low << " h:" << high << std::endl;
}
