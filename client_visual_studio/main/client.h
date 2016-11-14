#ifndef __Client_H__
#define __Client_H__

#include "stdafx.h"

#include <thread>
#include <utility>

#include "parser.h"
#include "thread_pool.h"
#include "util.h"

/* TODO: add substitutions of paresers, etc */
class Client
{
private:
	std::unique_ptr<Parser> parser;
	std::unique_ptr<ThreadPool> pool;

public:
	Client();

	Client(std::string file_name);
	~Client();

	void start();
};

#endif //__Client_H__
