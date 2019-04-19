CC = gcc
CFLAGS = -Wall -g

all: coremain monsieurpropre

coremain: api.o grammaire.o lecture.o coremain.o
	$(CC) $(CFLAGS) -o coremain coremain.o api.o grammaire.o lecture.o -lm

api: api.o grammaire.o lecture.o
	$(CC) $(CFLAGS) -o api api.o grammaire.o lecture.o -lm

coremain.o: coremain.c api.h lecture.h grammaire.h
	$(CC) $(CFLAGS) -c coremain.c

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
