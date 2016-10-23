#include <iostream>
#include <string>
#include <vector>

#include "fake_tcp.h"

void FakeTCP::init(std::string ip)
{
	printf("FakeTCP: init\n");
}

void FakeTCP::send(std::vector<int> data)
{
	printf("FakeTCP: send\n");
}