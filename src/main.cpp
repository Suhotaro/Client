#include <tinyxml.h>

#include "parser.h"
#include "client.h"

void use()
{
	printf("using:\n");
	printf("  ./src/client inputs/input.xml  num_threads\n\n");
	printf("num_threads: number of threads to be used by the application\n");
	printf("result will be written to \"outputs\" directory\n\n");
}

int main(int argc, char* argv[])
{
    std::string file_name;
    int num_threads = 4;

    if (argc == 1)
    {
    	use();
    	return 1;
    }
    else if (argc == 3)
    	num_threads = atoi(argv[2]);

    file_name = strdup(argv[1]);

	Server server(file_name, num_threads);
	server.start();

	return 0;
}

