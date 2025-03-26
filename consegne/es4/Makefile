all: server centro_vaccinale Fornitore



server:	server.o list.o
	gcc -Wall -o server server.o list.o

centro_vaccinale: centro_vaccinale.o
	gcc -Wall -o centro_vaccinale centro_vaccinale.o

Fornitore: Fornitore.o
	gcc -Wall -o Fornitore Fornitore.o



server.o: server.c list.h
	gcc -Wall -c server.c

centro_vaccinale.o: centro_vaccinale.c list.h
	gcc -Wall -c centro_vaccinale.c

Fornitore.o: Fornitore.c list.h
	gcc -Wall -c Fornitore.c
	
list.o: list.c list.h
	gcc -Wall -c list.c



clean:
	rm -f server centro_vaccinale Fornitore *o *~