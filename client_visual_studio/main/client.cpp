#include "stdafx.h"

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
	if (perr != PARSER_ERROR_NONE)
	{
		printf("init pareser failed\n");
		exit(0);
	}	

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
	
	std::cout << "Client: I have started working" << std::endl;

	std::shared_ptr<Job> job;
	while ((perr = parser->process(job)) == PARSER_ERROR_NONE)
	{
		printf("Client: schedule job\n");
		pool->schedule_job(job);
	}

	parser->deinit();
	
	std::this_thread::sleep_for (std::chrono::seconds(5));

	std::cout << "Client: I finished working" << std::endl;
}
