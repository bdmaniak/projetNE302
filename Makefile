api: api.o grammaire.o lecture.o
	gcc -Wall -o api api.o grammaire.o lecture.o -lm

api.o: api.c grammaire.h lecture.h
	gcc -Wall -c api.c

grammaire.o: grammaire.c api.h lecture.h
	gcc -Wall -c grammaire.c

lecture.o: lecture.c grammaire.h api.h
	gcc -Wall -c lecture.c

clean:
	rm *.o
