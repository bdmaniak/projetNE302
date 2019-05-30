all: 	
	gcc -I ../libparseur -I ../librequest-0.3/api  -L. -L ../libparseur -L ../librequest-0.3 -o server server.c -lparseur -lrequest -lm
