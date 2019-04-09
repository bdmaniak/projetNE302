CC = gcc
CFLAGS = -Wall -g

all: api monsieurpropre


api: api.o grammaire.o lecture.o
	$(CC) $(CFLAGS) -o api api.o grammaire.o lecture.o -lm

api.o: api.c grammaire.h lecture.h
	$(CC) $(CFLAGS) -c api.c

grammaire.o: grammaire.c api.h lecture.h
	$(CC) $(CFLAGS) -c grammaire.c

lecture.o: lecture.c grammaire.h api.h
	$(CC) $(CFLAGS) -c lecture.c

clean:
	rm api

monsieurpropre:
	rm *.o
