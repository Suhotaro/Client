#include <thread>
#include <iostream>

#include "server.h"
#include "parser.h"
#include "util.h"

Server::Server() {}
	
	/* TODO: make default parser */
Server::Server(Parser *concret_parser, int num_thread) :
		parser(concret_parser)
{
	parser_error perr = PARSER_ERROR_NONE;
	
	std::cout << "Server: I am being initialized" << std::endl;
	
	perr = parser->init();
	EXITIFTRUE(perr != PARSER_ERROR_NONE, "init pareser failed");
	
	pool = new ThreadPool(num_thread);
	EXITIFTRUE(pool == NULL, "allocate pool fail");	

	/* TODO: here must be demonization of server */
	
	printf("Server info:\n"
			"  num calculation threads: %d\n"
			"  file to be processed: %s\n",
			num_thread,
			parser->get_file_name().c_str());		
}

Server::~Server()
{
	delete parser;
	delete pool;
}

/* TODO: add substitutions of paresers, etc */
/* TODO: better giving file names to servers not to paresers only */
void Server::start()
{
	parser_error perr = PARSER_ERROR_NONE;
	int low, high;
	
	std::cout << "Server: I have started working" << std::endl;
		
	while ((perr = parser->process(low, high)) == PARSER_ERROR_NONE)
	{
		printf("Server: I am working: low:%d high:%d\n", low, high);
		
		pool->start_job(low, high);					
	}

	parser->deinit();

	std::cout << "Server: I finished working" << std::endl;
}