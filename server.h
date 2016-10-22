#ifndef __SERVER_H__
#define __SERVER_H__

#include <thread>

#include "parser.h"
#include "thread_pool.h"
#include "util.h"

/* TODO: add substitutions of paresers, etc */
/* TODO: better giving file names to servers not to paresers only */
/* TODO: make default parser */
class Server
{
private:
	/* TODO: remove *paser --> parser */
	Parser *parser;
	ThreadPool *pool;

public:
	Server();
	Server(Parser *concret_parser, int num_thread);
	~Server();

	void start();
};

#endif //__SERVER_H__
