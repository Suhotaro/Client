#ifndef __THREAD_POOL_H__
#define __THREAD_POOL_H__

#include <condition_variable>
#include <mutex>
#include <vector>

#include "buffer.h"

class ThreadPool
{
private:
	Buffer *buffers;
	int num_threads = 0;
	std::vector<std::thread> jobs;	

	std::mutex mtx;
	std::condition_variable all_buffers_captured;

	std::thread *puller;
	bool puller_run;

	bool are_there_free_buffers();

	/* calculates prime numbers for a range defined by low..high and stores
	 * result in buffers defined by buffer_idx */
	void calculate_prime_numbers(int buffer_idx, int low, int high);

	/* puller procedure */
	void pull();

public:
	ThreadPool(int num_threads);
	~ThreadPool();
	
	void start_job(int low, int high);
	void finish_job();
	
	void show();
};

#endif //__THREAD_POOL_H__
