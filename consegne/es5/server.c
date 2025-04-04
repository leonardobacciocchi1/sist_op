/*

		NON del tutto completo, 
		ho preso come ipotesi al massimo la combinazione di due visitatori
		ho dato una idea di gestione delle connsessioni ma non ho capito bene come gestirle
		perchè dovrei inviare guida non trovata e chiuderla e poi mi dice di gestirle??
		
		da sistemare insomma.

*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include <unistd.h>
#include <netdb.h>

#include "list.h"

#define BUF_SIZE 1000


int port = 8080;

double controllofattibile(int num, int num2){
	if(num - num2 < 0){
		return 0;
	}
	else return 1;
}

int main() 
{
	struct sockaddr_in serv_addr;
	struct sockaddr_in cli_addr;
	int max, min, num, count;
	LIST listguide = NewList();
	LIST listvisitatori = NewList();
	
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
	char buffer[BUF_SIZE], buffinv[256], buffinv1[256];
	char message [] ="guida trovata";
	int s ;
	while(1) 
	{
		
		printf("\n---\n: waiting connections\n");
		PrintList(listguide);
		PrintList(listvisitatori);
	
		
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
		printf("\n nuova connesione \n");

		char *str1 = strtok(buffer, " ");
		char *numero_str = strtok(NULL, " ");
		char *numero_str2 = strtok(NULL, " ");
		ItemType temp = {0};
		if (str1 != NULL && numero_str != NULL && numero_str2 != NULL) {
			printf("\n si è connesso una guida con: Nome: %s Num_min: %s Num_max: %s \n",str1, numero_str, numero_str2);
			min = atoi(numero_str);
			max = atoi(numero_str2);
			strncpy(temp.name, str1, sizeof(str1));
			temp.socket = newsockfd;
			temp.num_min= min;
			temp.num_max = max;
			listguide = EnqueueOrdered(listguide, temp);
			/*
			printf("si è connesso un fornitore con: Nome: %s Num: %s Min: %s \n",nome, numero_str, numero_str2);
			//printf("nome %s num %s min %s",nome, numero_str, numero_str2);
			num = atoi(numero_str);
			min = atoi(numero_str2);
			char bufferinv[256], bufferinv1[256];
			double fatt;
			//ora devo controllare se ci sono centri a cui assegnare vaccini del fornitore e dirglielo 

			ItemType* found_centro = FindFirstAvailable(listacentri, min);
							
				//con FindFirstAvailable guardo se ho trovato dei centri a cui assegnare
				//cioè dei centri che soddisfano le richieste del fornitore
				//con controllofattibile guardo se ne ho abbastanza di vaccini
				//allora invia la lista dei centri al fornitore e rimuove i centri soddisfatti
				//devo usare politica greedy, scelgo il C con la maggiore domanda non eccedente	
				// che sto facendo con EnqueueOrdered inserendoli gia per valore decrescente dei vaccini
			if (found_centro != NULL)
			{
				fatt = controllofattibile(found_centro->num_vaccini,temp.num_vaccini);
				printf("\n %d %d \n", found_centro->num_vaccini, temp.num_vaccini);
				if (fatt != 0)
				{
					printf("\n ho trovato un centro\n");
					snprintf(bufferinv, sizeof(bufferinv), "%s %d", found_centro->name, found_centro->socket);
					send(newsockfd, bufferinv, strlen(bufferinv), 0);
					snprintf(bufferinv1, sizeof(bufferinv1), "%s %d", temp.name, temp.socket);
					send(found_centro->socket, bufferinv1, strlen(bufferinv1), 0);
					temp.num_vaccini = temp.num_vaccini - found_centro->num_vaccini;
					if (temp.num_vaccini == 0)
					{
						listforni = Dequeue(listforni, temp);
					}
					listacentri= Dequeue(listacentri, *found_centro);
					close(newsockfd);
				}
				
			}
			
			// se non ci sono fornitori a cui dare i vaccini salva i dati e attende.
			else{
				printf("\n fornitore messo in attesa di centri compatibili \n");
				strncpy(temp.name, nome, sizeof(nome));
				temp.socket = newsockfd;
				temp.num_vaccini= num;
				temp.min_richieste = min;
				listforni = EnqueueOrdered(listforni, temp);
			}
				*/
		} 
		else
		{
			printf("\n si è connesso visitatore con Num: %s \n",str1);
			num = atoi(str1);
			ItemType* found_guida = FindFirstAvailable(listguide, num);
			if (found_guida != NULL)
			{
				printf("\n\n guida trovata \n \n");
				listguide = Dequeue(listguide,*found_guida);
				send(newsockfd, message, strlen(message), 0);
				
				//send numero a guida
				send(found_guida->socket, &num, sizeof(num),0);

			}
			else{
				
				
				//chiamo funzione che mi va a cercare nella lista di visitatori non soddisfatti
				//se insieme possono soddisfare la richiesta 
				
				//prima inseisco in attesa i visitatori
				
				ItemType* found_guida2 = Findthecoppia(listvisitatori,listguide, num);
				if (found_guida2 != NULL)
				{
					
					printf("\n\n guida trovata \n \n");
					
					listguide = Dequeue(listguide,*found_guida2);
					listvisitatori = DequeueFirst(listvisitatori);
					PrintList(listguide);
					PrintList(listvisitatori);
					
					send(newsockfd, message, strlen(message), 0);
					
					send(s, message, strlen(message), 0);
					
					send(found_guida2->socket, &num, sizeof(num),0);

				}
				else{
					printf("\n\n guida non trovata rimango in attesa\n \n");
					strncpy(temp.name, "", sizeof(""));
					temp.socket = newsockfd;
					s = newsockfd;
					temp.num_min = num;
					temp.num_max = 0;
					listvisitatori = EnqueueOrdered(listvisitatori, temp);
				}
			}
		}
	}
	return 0;
}



