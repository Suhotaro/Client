#ifndef __FAKE_TCP_H__
#define __FAKE_TCP_H__

#include <string>
#include <vector>
#include <fstream>
#include <memory>

class FakeTCP;

typedef enum
{
	ESTEBLISH = 1,
	RECONNECTION,
	SEND,
	SLOW,
	END,
} current_state;


/* XXX: I moved tcp states class's chain here due too circular dependencie issue */
class FakeTCPState
{
private:
	current_state state;

public:
	FakeTCPState(current_state state_) : state(state_) {}
	virtual ~FakeTCPState() {}

	virtual void step(FakeTCP &fake_tcp) = 0;
	current_state get_state() { return state; }
};

/* FakeTCPStateEstablish */
class FakeTCPStateEstablish : public FakeTCPState
{
public:
	FakeTCPStateEstablish();
	void  step(FakeTCP &fake_tcp) override final;
};


/* FakeTCPReconnect */
class FakeTCPReconnect : public FakeTCPState
{
private:
	enum {NUM_RECONNECTIONS = 3,};
	int num_reconnections;

public:
	FakeTCPReconnect();
	void  step(FakeTCP &fake_tcp) override final;
};


/* FakeTCPSend */
class FakeTCPSend : public FakeTCPState
{
public:
	FakeTCPSend() : FakeTCPState(SEND) {}
	~FakeTCPSend() {}

	void step(FakeTCP &fake_tcp) override final;
};


/* FakeTCPSlowPace */
class FakeTCPSlowPace : public FakeTCPState
{
public:
	FakeTCPSlowPace() : FakeTCPState(SLOW) {}
	~FakeTCPSlowPace() {}

	void step(FakeTCP &fake_tcp) override final;
};


/* FakeTCPEnd */
class FakeTCPEnd : public FakeTCPState
{
public:
	FakeTCPEnd() : FakeTCPState(END) {}
	virtual ~FakeTCPEnd() {}

	void step(FakeTCP &fake_tcp) override final;
};


class FakeTCP
{
private:
	std::string ip;
	/* file data will be written to */
	std::string file_name;

	std::unique_ptr<FakeTCPState> tcp_state;
	std::vector<int> data;

	enum { DEFAULT_DELAY = 4, };
	int max_delay = DEFAULT_DELAY;

public:
	FakeTCP(std::string ip_) :
		ip (ip_),
		file_name (get_new_file_name())
	{
		tcp_state =  std::unique_ptr<FakeTCPStateEstablish> (new FakeTCPStateEstablish());
	}

	~FakeTCP() {}

	void send();
	void set_data(std::vector<int> &data);
	std::vector<int> &get_data();

	/* TODO: make these functions private */

	std::string get_new_file_name();

	void set_state(std::unique_ptr<FakeTCPState> &tcp_state_)
	{
		tcp_state = std::move(tcp_state_);
	}

	void set_state(std::unique_ptr<FakeTCPSend> &tcp_state_)
	{
		tcp_state = std::move(tcp_state_);
	}

	void set_state(std::unique_ptr<FakeTCPReconnect> &tcp_state_)
	{
		tcp_state = std::move(tcp_state_);
	}

	void set_state(std::unique_ptr<FakeTCPSlowPace> &tcp_state_)
	{
		tcp_state = std::move(tcp_state_);
	}

	void set_state(std::unique_ptr<FakeTCPEnd> &tcp_state_)
	{
		tcp_state = std::move(tcp_state_);
	}
};



#endif //__FAKE_TCP_H__
