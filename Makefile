CC = gcc
CFLAGS = -g -Wall


all: server
	rm *.o

server: server.o socket.o
	$(CC) -I ../libparseur -I ../librequest-0.3/api  -L. -L ../libparseur -L ../librequest-0.3 -o server server.o socket.o -lparseur -lrequest -lm

server.o: server.c
	$(CC) $(CFLAGS) -c server.c

socket.o: socket.c
	$(CC) $(CFLAGS) -c socket.c