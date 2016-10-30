CC=g++
OPT=-Wall -Werror


all: client

client: main.o client.o parser.o thread_pool.o buffer.o fake_tcp.o joined_threads.o job.o
	${CC}  ${OPT} -std=c++11 main.o client.o parser.o thread_pool.o buffer.o fake_tcp.o joined_threads.o job.o -o client -pthread
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

joined_threads.o: joined_threads.cpp
	${CC}  ${OPT} -std=c++11 -c joined_threads.cpp

job.o : job.cpp
	${CC}  ${OPT} -std=c++11 -c job.cpp


clean:
	rm client *.o *~
