#include "parser.h"
#include "client.h"

void use()
{
	printf("using:\n");	
	printf("  ./src/client inputs/input.xml\n\n");
	printf("result will be written to \"outputs\" directory\n\n");
}

int main(int argc, char* argv[])
{
	std::string file_name;

	printf("  %s\n", argv[0]);

	if (argc == 1)
	{
		use();
		return 1;
	}

	file_name = argv[1];

	Client client(file_name);
	client.start();
		
	return 0;
}

