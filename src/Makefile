CC=g++
OPT=-Wall -Werror


all: client

client: main.o client.o parser.o thread_pool.o buffer.o fake_tcp.o
	${CC}  ${OPT} -std=c++11 main.o client.o parser.o thread_pool.o buffer.o fake_tcp.o -o client -pthread
	rm *.o

main.o: main.cpp
	${CC}  ${OPT} -std=c++11 -c main.cpp

client.o: client.cpp
	${CC}  ${OPT} -std=c++11 -c client.cpp

parser.o: parser.cpp
	${CC}  ${OPT} -std=c++11 -c parser.cpp

thread_pool.o: thread_pool.cpp
	${CC}  ${OPT} -std=c++11 -c thread_pool.cpp

buffer.o : buffer.cpp
	${CC}  ${OPT} -std=c++11 -c buffer.cpp

fake_tcp.o: fake_tcp.cpp
	${CC}  ${OPT} -std=c++11 -c fake_tcp.cpp

clean:
	rm client *.o *~
