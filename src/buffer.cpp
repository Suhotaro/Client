#include <thread>
#include <queue>
#include <mutex>

#include "buffer.h"

void Buffer::set_used()
{
	std::lock_guard<std::mutex> guard(mtx);
	used = true;
}

void Buffer::set_unused()
{
	std::lock_guard<std::mutex> guard(mtx);
	used = false;
}

bool Buffer::is_used()
{
	std::lock_guard<std::mutex> guard(mtx);
	return used;
}

void Buffer::add_back(int val)
{
	std::lock_guard<std::mutex> guard(mtx);
	queue.push_back(val);
}

int Buffer::get_front()
{
	int val;
	
	std::lock_guard<std::mutex> guard(mtx);
	val = queue.front();
	queue.pop_front();

	return val;
}

bool Buffer::is_empty()
{
	std::lock_guard<std::mutex> guard(mtx);	
	return queue.empty();
}