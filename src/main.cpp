#include <tinyxml.h>

#include "parser.h"
#include "client.h"

int main(int argc, char* argv[])
{
    std::string file_name;

    if (argc == 1)
        file_name = "../inputs/input.xml";
    else
        file_name = strdup(argv[1]);

	Server server(new XMLParser(file_name), 4);
	server.start();

	return 0;
}

