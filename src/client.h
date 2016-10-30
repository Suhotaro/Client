#ifndef __SERVER_H__
#define __SERVER_H__

#include <thread>
#include <utility>

#include "parser.h"
#include "thread_pool.h"
#include "util.h"

/* TODO: add substitutions of paresers, etc */
class Server
{
private:
	std::unique_ptr<Parser> parser;
	std::unique_ptr<ThreadPool> pool;

public:
	Server();

	Server(std::string file_name, int num_thread);
	~Server();

	void start();
};

#endif //__SERVER_H__
