# Makefile for client and server

all: client server start

server: server.cpp utils.cpp ticketOffice.cpp logServer.cpp
		g++ -o server -Wall -Wextra server.cpp utils.cpp ticketOffice.cpp logServer.cpp -std=c++11 -D_REENTRANT -lpthread -lrt

client: client.cpp utils.cpp logClient.cpp ticketOffice.cpp
		g++ -o client -Wall -Wextra client.cpp utils.cpp logClient.cpp ticketOffice.cpp -std=c++11

start: start.c
	   gcc -o start -Wall -Wextra start.c

clean: 
	rm -f client server start