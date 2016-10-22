#ifndef __THREAD_POOL_H__
#define __THREAD_POOL_H__

#include <condition_variable>
#include <mutex>

#include "buffer.h"

class ThreadPool
{
private:
	Buffer *buffers;
	int num_threads = 0;

	std::mutex mtx;
	std::condition_variable all_buffers_captured;
	
	bool are_there_free_buffers();
	
public:
	ThreadPool(int num_threads);
	~ThreadPool();
	
	void start_job(int low, int high);
};

#endif //__THREAD_POOL_H__
