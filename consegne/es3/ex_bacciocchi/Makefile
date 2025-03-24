all:	client server 

client:	client.c
	gcc -Wall -o client client.c

server:	server.c list.o
	gcc -Wall -o server server.c list.o

clean:
	rm -f client server *~

