#include <thread>
#include <iostream>

#include "server.h"
#include "parser.h"
#include "util.h"

Server::Server() {}
	
	/* TODO: make default parser */
Server::Server(Parser *concret_parser) : parser(concret_parser)
{
	num_cpus = std::thread::hardware_concurrency();
	
	printf("Server info:\n"
			"  num cpus: %d\n"
			"  file to be processed: %s\n",
			num_cpus,
			parser->get_file_name().c_str());
}

Server::~Server()
{
	delete parser;
}
	
/* TODO: add substitutions of paresers, etc */
/* TODO: better giving file names to servers not to paresers only */
void Server::start()
{
	parser_error perr = PARSER_ERROR_NONE;
	int low, high;
	
	std::cout << "I have started working" << std::endl;	
	
	perr = parser->init();
	EXITIFTRUE(perr != PARSER_ERROR_NONE, "init pareser failed");

	while ((perr = parser->process(low, high)) == PARSER_ERROR_NONE)
	{
		printf("I am working: low:%d high:%d\n", low, high);
	}

	parser->deinit();

	std::cout << "I finished working" << std::endl;
}