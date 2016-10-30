#ifndef __BUFFER_H__
#define __BUFFER_H__

#include <deque>
#include <thread>
#include <mutex>

class Buffer
{
private:
	std::deque<int> queue;
	std::mutex queue_mutex;

	bool used = false;
	std::mutex used_mutex;
	
public:
	Buffer(): used(false) {}
	~Buffer() {}

    Buffer(const Buffer& buffer) : used(buffer.used) {}
    Buffer& operator=(const Buffer& buffer)
    {
    	used = buffer.used;
    	return *this;
    }

	void set_used();
	void set_unused();
	bool is_used();

	void add_back(int val);
	int get_front();
	bool is_empty();
};

#endif //__BUFFER_H__
