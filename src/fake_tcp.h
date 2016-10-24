#ifndef __FAKE_TCP_H__
#define __FAKE_TCP_H__

#include <string>
#include <vector>
#include <fstream>

class FakeTCP
{
private:
	std::string ip;
	/* file data will be written to */
	std::string file_name;
	std::ofstream of;

	int max_delay = 4;

	std::string get_new_file_name();
	bool create_socket();
	bool connect();

public:
	FakeTCP() {file_name = get_new_file_name();}
	~FakeTCP() {}

	void init(std::string ip);
	void send(std::vector<int> data);
};

#endif //__FAKE_TCP_H__
