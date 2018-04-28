# Makefile for client and server

all: client server

client: client.cpp utils.cpp logClient.cpp 
		g++ -o client -Wall -Wextra client.cpp utils.cpp logClient.cpp -std=c++11

server: server.cpp utils.cpp ticketOffice.cpp logServer.cpp
		g++ -o server -Wall -Wextra server.cpp utils.cpp ticketOffice.cpp logServer.cpp -std=c++11

clean: 
	rm -f client server