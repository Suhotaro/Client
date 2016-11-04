#include "fake_tcp.h"
#include "util.h"

FakeTCPStateEstablish::FakeTCPStateEstablish() :
	FakeTCPState(ESTEBLISH)
{
}

void FakeTCPStateEstablish::step(FakeTCP &fake_tcp)
{
	printf("TCPState: ESTEBLISH:%d\n", get_state());
		
	/* XXX: Open a socket if succes set Send state */
	if (true)
	{
		std::unique_ptr<FakeTCPSend> send = std::unique_ptr<FakeTCPSend> (new FakeTCPSend());
		fake_tcp.set_state(send);	
	}
	else
	{
		/* XXX: If open failed set Reconnection state */
		std::unique_ptr<FakeTCPReconnect> reconnect = std::unique_ptr<FakeTCPReconnect> (new FakeTCPReconnect());
		fake_tcp.set_state(reconnect);
	}			
}


FakeTCPReconnect::FakeTCPReconnect() :
	FakeTCPState(RECONNECTION), num_reconnections(NUM_RECONNECTIONS)
{
}

void FakeTCPReconnect::step(FakeTCP &fake_tcp)
{
	printf("TCPState: RECONNECT:%d\n", get_state());
	
	/* Try reconnect to a server/client */
	for (int i = 0; i < num_reconnections; i++)
	{
		/* XXX: Open a socket if succes set Send state */
		if (true)
		{
			std::unique_ptr<FakeTCPSend> send = std::unique_ptr<FakeTCPSend> (new FakeTCPSend());
			fake_tcp.set_state(send);
			
			return;
		}
	}

	/* XXX: If failed num_reconnections times set End state */
	std::unique_ptr<FakeTCPEnd> end = std::unique_ptr<FakeTCPEnd> (new FakeTCPEnd());
	fake_tcp.set_state(end);
}

void FakeTCPSend::step(FakeTCP &fake_tcp)
{
	printf("TCPState: SEND:%d\n", get_state());

	std::string file_name = fake_tcp.get_new_file_name();
	
	std::ofstream of;
	
	of.open(file_name);
	EXITIFTRUE(of.fail(), "open output \"%s\" file failed", file_name.c_str());
	
	std::vector<int> &data = fake_tcp.get_data();
	int num_chunks = 1;
	
	/* XXX: Do chunking if data is big */
	for (int i = 0; i < num_chunks; i++)
	{
		of << "<root>\n";
		of << "  <primes> ";
		for (size_t i = 0; i < data.size(); i++)
			of << data[i] << " ";
		of << "  </primes>\n";
		of << "</root>";			
	}
	
	of.close();
	
	/* XXX: if send OK set End status */
	std::unique_ptr<FakeTCPEnd> end = std::unique_ptr<FakeTCPEnd> (new FakeTCPEnd());
	fake_tcp.set_state(end);
}

void FakeTCPSlowPace::step(FakeTCP &fake_tcp)
{
	printf("TCPState: Slow Pace:%d\n", get_state());
	
	/* TODO: I do no clearaly undestend SlowPace */
}

void FakeTCPEnd::step(FakeTCP &fake_tcp)
{
	printf("TCPState: End:%d\n", get_state());
	
	/* XXX: close connection here */
}