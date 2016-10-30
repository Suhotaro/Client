#ifndef __JOINED_THREADS_H__
#define __JOINED_THREADS_H__

#include <thread>
#include <vector>

/* RAII like class maiden to be sure, that all threads wich are stored in 
 * JoinedThreads object will be finished (joined) in case of object destuction
 * or in case of an exeption.
 */
class JoinedThreads
{
private:
    std::vector<std::thread> &threads;

public:
    JoinedThreads();
    JoinedThreads(std::vector<std::thread> &threads_);
    ~JoinedThreads();
};

class JoinedThread
{
private:
	std::unique_ptr<std::thread> &thread;

public:
	JoinedThread();
	JoinedThread(std::unique_ptr<std::thread> &thread_);
    ~JoinedThread();
};

#endif //__JOINED_THREADS_H__