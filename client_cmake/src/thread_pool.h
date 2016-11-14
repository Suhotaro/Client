#ifndef __THREAD_POOL_H__
#define __THREAD_POOL_H__

#if defined(WIN32) || defined(_WIN32)
#include "stdafx.h"
#endif

#include <vector>
#include <queue>
#include <thread>
#include <map>
#include <mutex>
#include <atomic>
#include <condition_variable>

#include "buffer.h"
#include "joined_threads.h"
#include "job.h"

class ThreadPool
{
private:
	enum {THREADS_PER_CPU = 10,
		  MAX_RANGE = 20,};

	int threads_per_cpu = THREADS_PER_CPU;
	int max_range = MAX_RANGE;		

	/* Work threads */
	std::atomic_bool works_run;
	std::mutex works_queue_mutex;
	std::deque<std::shared_ptr<Job>> works_queue;
	std::vector<std::thread> works_threads;
	JoinedThreads works_threads_joiner;
	std::map<std::thread::id, Buffer> buffers;
	
	void worker_thread();
	bool do_jobs();
	
	/* Puller */
	std::unique_ptr<std::thread> puller;
	JoinedThread puller_joiner;
	bool puller_run;

	void pull();
	void collect_data_and_send();
	void sender_thread(std::vector<int> &data);
	

	/* Statistic */
	std::map<std::thread::id, int> statistic;
	std::mutex statistic_mutex;

	void statistic_collect(int num);	

public:
	ThreadPool();
	~ThreadPool();
	
	void schedule_job(std::shared_ptr<Job> job);
	void inspect_jobs();

	void statistic_show();
};

#endif //__THREAD_POOL_H__

