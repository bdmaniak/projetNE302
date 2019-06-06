all: server
	rm *.o

server: server.o socket.o
	gcc -I ../libparseur -I ../librequest-0.3/api  -L. -L ../libparseur -L ../librequest-0.3 -o server server.o socket.o -lparseur -lrequest -lm

server.o: server.c
	gcc -g -Wall -c server.c

socket.o: socket.c
	gcc -g -c socket.c