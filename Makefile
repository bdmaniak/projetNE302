all: 	
	gcc -I ../libparser -I ../librequest-0.3/api  -L. -L ../libparser -L ../librequest-0.3 -o server server.c -lparser -lrequest -lm -g 
