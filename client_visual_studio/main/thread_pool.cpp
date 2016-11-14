#include "stdafx.h"

#include <iostream>
#include <mutex>
#include <condition_variable>
#include <atomic>
#include <vector>

#include "thread_pool.h"
#include "buffer.h"
#include "util.h"
#include "fake_tcp.h"

ThreadPool::ThreadPool() : works_run(true),
works_threads_joiner(works_threads), puller_joiner(puller)
{
	const unsigned int thread_count = std::thread::hardware_concurrency();

	printf("POOL: num of workers threads %d\n", thread_count);

	for (unsigned int i = 0; i < thread_count; i++)
	{
		works_threads.push_back(std::thread(&ThreadPool::worker_thread, this));
		buffers.insert( std::pair<std::thread::id, Buffer>(works_threads[i].get_id(), Buffer()));
		statistic.insert( std::pair<std::thread::id, int>(works_threads[i].get_id(), 0));
	}

	/* TODO: it has to be effciently calculated */
	//threads_per_cpu = 20;

	puller = std::unique_ptr<std::thread> (new std::thread(std::thread(&ThreadPool::pull, this)));
	puller_run = true;
}

ThreadPool::~ThreadPool()
{
	works_run = false;
	
	/* Wait until all threads are stoped */
	for (unsigned int i = 0; i < works_threads.size(); i++)
		if (works_threads[i].joinable())
			works_threads[i].join();
	
	puller_run = false;

	if (puller->joinable())
		puller->join();

	statistic_show();
}

void ThreadPool::worker_thread()
{
	printf("POOL: start work thread\n");

	while(works_run)
	{
		if (false == do_jobs())
			std::this_thread::yield();
	}

	/* If there are yet not finished jobs remaining process them all */
	while(true)
	{
		works_queue_mutex.lock();

		if (works_queue.empty())
		{
			works_queue_mutex.unlock();
			printf("POOL: stop work thread\n");

			return;
		}

		std::shared_ptr<Job> job = works_queue.front();
		works_queue.pop_front();

		works_queue_mutex.unlock();

		Buffer &buffer = buffers[std::this_thread::get_id()];
		job->do_job();
		job->collect_data(buffer);
	}
}

bool ThreadPool::do_jobs()
{
	works_queue_mutex.lock();

	if (works_queue.empty())
	{
		works_queue_mutex.unlock();
		return false;
	}

	std::vector<std::shared_ptr<Job>> subjobs;

	int tmp = threads_per_cpu;	
	while (tmp != 0)
	{
		if (works_queue.empty())
			break;
		
		subjobs.push_back(works_queue.front());
		works_queue.pop_front();

		tmp -= 1;
	}

	works_queue_mutex.unlock();

	statistic_collect(subjobs.size());

	std::vector<std::thread> sub_workers_threads;

	for (unsigned int i = 0; i < subjobs.size(); i++)
		sub_workers_threads.push_back(std::thread(&Job::do_job, subjobs[i]));		

	Buffer &buffer = buffers[std::this_thread::get_id()];

	for (unsigned int i = 0; i < subjobs.size(); i++)
	{
		sub_workers_threads[i].join();
		subjobs[i]->collect_data(buffer);
	}
			
	return true;
}

/* public functions */
void ThreadPool::schedule_job(std::shared_ptr<Job> job)
{
	std::lock_guard<std::mutex> guard(works_queue_mutex);
	std::vector<std::shared_ptr<Job>> v(job->divide_job());
	for (unsigned int i = 0; i < v.size(); i++)
		works_queue.push_back(v[i]);
}

void ThreadPool::inspect_jobs()
{
#if 0
	/* TODO: show each Job content */
#endif
}

void ThreadPool::collect_data_and_send()
{
	for(auto iterator = buffers.begin(); iterator != buffers.end(); iterator++)
	{
		std::vector<int> data;
		Buffer &buffer = iterator->second;

		/* Pull calculated prime numbers from a buffer */
		while (!buffer.is_empty())
			data.push_back(buffer.get_front());

		/* send data via fake tcp */
		if (!data.empty())
		{
			printf("POOL: send data ------>>\n");

			std::thread sender(&ThreadPool::sender_thread, this, std::ref(data));
			sender.join();
		}
	}
}

void ThreadPool::pull()
{
	printf("POOL: start pull +++++++>>\n");

	while (puller_run)
		collect_data_and_send();

	/* After that point all worker threads are already stoped, thus
	 * send data that left in our buffers
	 */
	collect_data_and_send();

	printf("POOL: stop pull <<+++++++\n");
}

void ThreadPool::sender_thread(std::vector<int> &data)
{
	FakeTCP tcp("127.0.0.1:80");
	tcp.set_data(data);
	tcp.send();
}

void ThreadPool::statistic_collect(int num)
{
	statistic_mutex.lock();

	int tmp = statistic[std::this_thread::get_id()];
	statistic.erase(std::this_thread::get_id());
	statistic.insert( std::pair<std::thread::id, int>(std::this_thread::get_id(), tmp+num));

	/* Doing it just to better remember unique_lock feture */
	statistic_mutex.unlock();
}

void ThreadPool::statistic_show()
{
	printf("-----------------------------\n\n");
	printf("Statistic:\n");
	int j = 1;
	for (auto i = statistic.begin(); i != statistic.end(); i++)
	{
		printf("  thread : %d\n", i->second);
		j++;
	}
	printf( "-----------------------------\n\n");
}
