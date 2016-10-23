all: client

client: main.o server.o parser.o thread_pool.o buffer.o
	g++ -std=c++11 main.o server.o parser.o thread_pool.o buffer.o -o client -pthread
	rm *.o

main.o: main.cpp
	g++ -std=c++11 -c main.cpp

server.o: server.cpp
	g++ -std=c++11 -c server.cpp

parser.o: parser.cpp
	g++ -std=c++11 -c parser.cpp

thread_pool.o: thread_pool.cpp
	g++ -std=c++11 -c thread_pool.cpp

buffer.o : buffer.cpp
	g++ -std=c++11 -c buffer.cpp


clean:
	rm client *.o *~
