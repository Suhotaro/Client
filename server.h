#ifndef __SERVER_H__
#define __SERVER_H__

#include "parser.h"
#include "util.h"
#include <thread>

class Server
{
private:
	Parser *parser;
	int num_cpus = 0;
	
public:
	Server() {}
	
	/* TODO: make default parser */
	Server(Parser *concret_parser) : parser(concret_parser) {

		num_cpus = std::thread::hardware_concurrency();

		printf("Server info:\n"
				"  num cpus: %d\n"
				"  file to be processed: %s\n",
				num_cpus,
				parser->get_file_name().c_str());

	}
	~Server() {
		delete parser;
	}
	
	/* TODO: add substitutions of paresers, etc */
	/* TODO: better giving file names to servers not to paresers only */

	void start()
	{
		parser_error perr = PARSER_ERROR_NONE;
		std::cout << "I have started working" << std::endl;
		
		int low, high;
		
		perr = parser->init();
		EXITIFTRUE(perr != PARSER_ERROR_NONE, "init pareser failed");

		while ((perr = parser->process(low, high)) == PARSER_ERROR_NONE)
		{
			printf("I am working: low:%d high:%d\n", low, high);
		}

		parser->deinit();

		std::cout << "I finished working" << std::endl;
	}
};

#endif //__SERVER_H__
