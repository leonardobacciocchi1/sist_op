all: server guide visitatori



server:	server.o list.o
	gcc -Wall -o server server.o list.o

guide: guide.o
	gcc -Wall -o guide Guide.o

visitatori: visitatori.o
	gcc -Wall -o visitatori Visitatori.o



server.o: server.c list.h
	gcc -Wall -c server.c

guide.o: Guide.c list.h
	gcc -Wall -c Guide.c

visitatori.o: Visitatori.c list.h
	gcc -Wall -c Visitatori.c
	
list.o: list.c list.h
	gcc -Wall -c list.c



clean:
	rm -f server guide visitatori *o *~
