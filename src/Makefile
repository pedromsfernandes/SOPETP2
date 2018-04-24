# Makefile for client and server

all: client server

client: client.cpp utils.cpp 
		g++ -o client -Wall -Wextra client.cpp utils.cpp -std=c++11

server: server.cpp utils.cpp 
		g++ -o server -Wall -Wextra server.cpp utils.cpp -std=c++11

clean: 
	rm -f client server