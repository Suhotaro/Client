#include <thread>
#include <iostream>

#include "client.h"
#include "parser.h"
#include "util.h"

Client::Client(std::string file_name) {
	parser_error perr = PARSER_ERROR_NONE;

	if (file_name.empty())
		return;

	parser = std::unique_ptr<Parser>(new XMLParser(file_name));
	pool = std::unique_ptr<ThreadPool>(new ThreadPool());

	perr = parser->init();
	EXITIFTRUE(perr != PARSER_ERROR_NONE, "init pareser failed");

	printf( "-----------------------------\n"
			"Client info:\n"
			"  file to be processed: %s\n"
			"-----------------------------\n\n",
			parser->get_file_name().c_str());
}

Client::~Client()
{
}

/* TODO: add substitutions of paresers, etc */
/* TODO: better giving file names to Clients not to paresers only */
void Client::start()
{
	parser_error perr = PARSER_ERROR_NONE;
	int low, high;
	
	std::cout << "Client: I have started working" << std::endl;
		
	while ((perr = parser->process(low, high)) == PARSER_ERROR_NONE)
	{
		printf("Client: I am working: low:%d high:%d\n", low, high);

		pool->start_job(low, high);
	}
	
	parser->deinit();
	
	//std::this_thread::sleep_for (std::chrono::seconds(2));

	std::cout << "Client: I finished working" << std::endl;
}
