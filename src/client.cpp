#include <thread>
#include <iostream>

#include "client.h"
#include "parser.h"
#include "util.h"

Server::Server(std::string file_name, int num_thread) {
	parser_error perr = PARSER_ERROR_NONE;

	if (file_name.empty())
		return;

	parser = std::unique_ptr<Parser>(new XMLParser(file_name));
	pool = std::unique_ptr<ThreadPool>(new ThreadPool(num_thread));

	perr = parser->init();
	EXITIFTRUE(perr != PARSER_ERROR_NONE, "init pareser failed");

	printf( "-----------------------------\n"
			"Server info:\n"
			"  num calculation threads: %d\n"
			"  file to be processed: %s\n"
			"-----------------------------\n\n",
			num_thread,
			parser->get_file_name().c_str());
}

Server::~Server()
{
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

	pool->wait_finishing_job();
	
	parser->deinit();
	
	/* XXX: remove it */
	//std::this_thread::sleep_for (std::chrono::seconds(5));
	
	//pool->show();
	
	std::cout << "Server: I finished working" << std::endl;
}
