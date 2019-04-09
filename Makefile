all: api monsieurpropre


api: api.o grammaire.o lecture.o
	gcc -Wall -g -o api api.o grammaire.o lecture.o -lm

api.o: api.c grammaire.h lecture.h
	gcc -Wall -g -c api.c

grammaire.o: grammaire.c api.h lecture.h
	gcc -Wall -g -c grammaire.c

lecture.o: lecture.c grammaire.h api.h
	gcc -Wall -g -c lecture.c

clean:
	rm api

monsieurpropre:
	rm *.o
