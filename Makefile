all: client

client: main.o client.o parser.o thread_pool.o buffer.o fake_tcp.o
	g++ -std=c++11 main.o client.o parser.o thread_pool.o buffer.o fake_tcp.o -o client -pthread
	rm *.o

main.o: main.cpp
	g++ -std=c++11 -c main.cpp

client.o: client.cpp
	g++ -std=c++11 -c client.cpp

parser.o: parser.cpp
	g++ -std=c++11 -c parser.cpp

thread_pool.o: thread_pool.cpp
	g++ -std=c++11 -c thread_pool.cpp

buffer.o : buffer.cpp
	g++ -std=c++11 -c buffer.cpp

fake_tcp.o: fake_tcp.cpp
	g++ -std=c++11 -c fake_tcp.cpp


clean:
	rm client *.o *~
