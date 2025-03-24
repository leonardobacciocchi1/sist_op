all:	client server 

client:	client.c
	gcc -Wall -o client client.c

client1: client_editrice.c
	gcc -Wall -o client1 client_editrice.c

server:	server.c list.o
	gcc -Wall -o server server.c list.o

clean:
	rm -f client server *~

