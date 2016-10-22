#include <tinyxml.h>
#include "parser.h"
#include "server.h"

int main(int argc, char* argv[])
{
	Server server(new XMLParser("input.xml"));
	server.start();

	return 0;
}

