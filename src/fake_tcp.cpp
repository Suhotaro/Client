#include <iostream>
#include <string>
#include <vector>

#include "fake_tcp.h"
#include "util.h"

std::string FakeTCP::get_new_file_name()
{
	static int num = 0;
	++num;

	std::string file_name("outputs/output");
	std::string suffix(".xml");

	file_name += std::to_string(num) + suffix;

	return file_name;
}

bool FakeTCP::create_socket()
{
	/* fd = socket(...); */

	return true;
}

bool FakeTCP::connect()
{
	for(int sec = 1; sec <= max_delay; sec <<= 1)
	{
		/* connect( fd, ...); */
		;
	}

	return true;
}

void FakeTCP::init(std::string ip)
{
	bool ret;

	ret = create_socket();
	RETURNIFTRUE(ret == false, "create socket failed");

	ret = connect();
	RETURNIFTRUE(ret == false, "connect to srver failed");
}

void FakeTCP::send(std::vector<int> data)
{
	printf("FakeTCP: send\n");

	/* write(fd, ...) */

	of.open(file_name);
	EXITIFTRUE(of.fail(), "open output \"%s\" file failed", file_name.c_str());

	of << "<root>\n";
	of << "  <primes> ";
	for (int i = 0; i < data.size(); i++)
		of << data[i] << " ";
	of << "  </primes>\n";
	of << "</root>";

	of.close();
}
