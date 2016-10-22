all: server

server: main.o server.o parser.o
	g++ -std=c++11 main.o server.o parser.o -o server -pthread

main.o: main.cpp
	g++ -std=c++11 -c main.cpp

server.o: server.cpp
	g++ -std=c++11 -c server.cpp

parser.o: parser.cpp
	g++ -std=c++11 -c parser.cpp

clean:
	rm server *.o *~
