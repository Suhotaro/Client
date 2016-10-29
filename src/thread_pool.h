#ifndef __THREAD_POOL_H__
#define __THREAD_POOL_H__

#include <condition_variable>
#include <mutex>
#include <vector>

#include "buffer.h"

class ThreadPool
{
private:

	std::vector<Buffer> buffers;

	enum { NUM_DEFAULT_THREADS = 4,};
	int num_threads = NUM_DEFAULT_THREADS;
	std::vector<std::thread> jobs;

	std::mutex mtx;
	std::condition_variable all_buffers_captured;

	std::unique_ptr<std::thread> puller;
	bool puller_run;

	bool are_there_free_buffers();

	/* calculates prime numbers for a range defined by low..high and stores
	 * result in buffers defined by buffer_idx */
	void calculate_prime_numbers(int buffer_idx, int low, int high);

	/* puller procedure */
	void pull();
	
	/* sender procedure */
	void send(std::vector<int> &data);

public:
	ThreadPool(int num_threads);
	~ThreadPool();
	
	void start_job(int low, int high);
	void wait_finishing_job();
	
	void show();
};

#endif //__THREAD_POOL_H__
