#ifndef __FAKE_TCP_H__
#define __FAKE_TCP_H__

#include <string>
#include <vector>

class FakeTCP
{
private:
	std::string ip;

public:
	FakeTCP() {}
	~FakeTCP() {}

	void init(std::string ip);
	void send(std::vector<int> data);
};

#endif //__FAKE_TCP_H__