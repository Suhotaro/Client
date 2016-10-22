#ifndef __SERVER_H__
#define __SERVER_H__

#include "parser.h"
#include "util.h"
#include <thread>

/* TODO: add substitutions of paresers, etc */
/* TODO: better giving file names to servers not to paresers only */
/* TODO: make default parser */
class Server
{
private:
	Parser *parser;
	int num_cpus = 0;
	

public:
	Server();
	Server(Parser *concret_parser);
	~Server();

	void start();
};

#endif //__SERVER_H__
