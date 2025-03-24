#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include <unistd.h>
#include <netdb.h>

#include "list.h"

#define BUF_SIZE 1000


int port = 8080;


int main() 
{
	struct sockaddr_in serv_addr;
	struct sockaddr_in cli_addr;
	int num, ret;
	LIST listclienti = NewList();
	LIST listalibri = NewList();
	ItemType temp;
	int  newsockfd;
	// Socket opening
	int sockfd = socket( PF_INET, SOCK_STREAM, 0 );  
	if ( sockfd == -1 ) 
	{
		perror("Error opening socket");
		exit(1);
	}
	
	int options = 1;
	if(setsockopt (sockfd, SOL_SOCKET, SO_REUSEADDR, &options, sizeof (options)) < 0) {
		perror("Error on setsockopt");
		exit(1);
	}

	bzero( &serv_addr, sizeof(serv_addr) );
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = INADDR_ANY;
	serv_addr.sin_port = htons(port);

	// Address bindind to socket
	if ( bind( sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr) ) == -1 ) 
	{
		perror("Error on binding");
		exit(1);
	}
	
	// Maximum number of connection kept in the socket queue
	if ( listen( sockfd, 20 ) == -1 ) 
	{
		perror("Error on listen");
		exit(1);
	}


	socklen_t address_size = sizeof( cli_addr );	
	char buffer[BUF_SIZE];
	char message[20];
	while(1) 
	{
		printf("\n---\n: waiting connections\n");
		
		// New connection acceptance		
		newsockfd = accept( sockfd, (struct sockaddr *)&cli_addr, &address_size );      
		if (newsockfd == -1) 
		{
			perror("Error on accept");
			exit(1);
		}
		
		bzero(buffer, BUF_SIZE);
		if (recv(newsockfd, buffer, sizeof(buffer), 0 ) == -1) {
		    perror("Error on receive");
			close(newsockfd);
		    exit(1);
		}
		printf("nuova connesione");

		char *nome = strtok(buffer, " ");
		char *numero_str = strtok(NULL, " ");

		if (nome != NULL && numero_str != NULL) {
			printf("si è connessa la casa editrice");
			num = atoi(numero_str);
			printf("Dati ricevuti: Nome = %s, Numero = %d\n", nome, num);
			
			strncpy(temp.name, nome, sizeof(temp.name) -1);
			temp.name[sizeof(temp.name) - 1] = '\0';
			temp.socket = newsockfd;
			temp.num_copie = num;
			listalibri = EnqueueLast(listalibri, temp);
			ItemType* found_client = Find(listclienti, temp);
			ItemType* found_lib = Find(listalibri,temp);
			if (found_client != NULL) {
				
				listclienti = Dequeue(listclienti, *found_client);
				found_lib->num_copie = found_lib->num_copie-1;
				
				if (found_lib->num_copie == 0){listalibri = Dequeue(listalibri,*found_lib);}
				strcpy(message, "trovato");
				send(found_client->socket, message, strlen(message), 0);
				close(temp.socket);
			}
			
			close(temp.socket);
		} 
		else if (nome != NULL && numero_str == NULL)
		{
			printf("si è connesso un cliente");
			strncpy(temp.name, nome, sizeof(temp.name) -1);
			temp.name[sizeof(temp.name) - 1] = '\0';
			temp.socket = newsockfd;
			temp.num_copie = 0;
			listclienti = EnqueueLast(listclienti, temp);
			printf("Nuovo cliente registrato e connesso: %s\n", temp.name);
			
			ItemType* found_libro = Find(listalibri, temp);
			if (found_libro != NULL) {
				listclienti = Dequeue(listclienti, *found_libro);
				found_libro->num_copie = found_libro->num_copie-1;
				
				if (found_libro->num_copie == 0){listalibri = Dequeue(listalibri,*found_libro);}
				strcpy(message, "trovato");
				send(temp.socket, message, strlen(message), 0);
				close(temp.socket);
			}		
		}
		else{
			printf("Errore nella conversione dei dati ricevuti.\n");
		}
		PrintList(listalibri);
		PrintList(listclienti);
		
	}
	return 0;
}



