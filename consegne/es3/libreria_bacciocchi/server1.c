#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <netdb.h>

#include <time.h>

#include "list.h"

#define BUF_SIZE 1000

// modificabile, indica numero giocatori
#define MAX_PLAYERS 3

//porta 8000 mi da problemi meglio 8080
int port = 8080;

int main()
{
    //dichiaro liste una per il registro e una per i giocatori colegati
    LIST registered_players = NewList();
    LIST connected_players = NewList();
    //inizializzo random e socket
    srand(time(NULL));
	int newsockfd;
    //Itemtype con nome punteggio numero partite e socket
	ItemType temp;

    //inizializzo socket e indirizzi
    struct sockaddr_in serv_addr, cli_addr;

    int sockfd = socket(PF_INET, SOCK_STREAM, 0);

    if (sockfd == -1)
    {
        perror("Error opening socket");
        exit(1);
    }

    int options = 1;

    if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &options, sizeof(options)) < 0)
    {
        perror("Error on setsockopt");
        exit(1);
    }

    bzero(&serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(port);

    if (bind(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) == -1)
    {
        perror("Error on binding");
        exit(1);
    }

    if (listen(sockfd, 20) == -1)
    {
        perror("Error on listen");
        exit(1);
    }

    socklen_t address_size = sizeof(cli_addr);
    char buf[BUF_SIZE];

	while (1) 
    {
	    printf("\nIn attesa di una nuova connessione...\n");

	    int newsockfd = accept(sockfd, (struct sockaddr *)&cli_addr, &address_size);
	    if (newsockfd == -1) {
	        perror("Errore nell'accept");
	        continue;
	    }

	    bzero(buf, BUF_SIZE);

	    if (recv(newsockfd, buf, BUF_SIZE, 0) == -1) {
	        perror("Errore nella ricezione");
	        close(newsockfd);
	        continue;
	    }

	    printf("Nuovo giocatore connesso: %s\n", buf);

        // prendo nome utente e importo tutto a 0
	    ItemType temp;
	    strncpy(temp.name, buf, sizeof(temp.name));
	    temp.name[sizeof(temp.name) - 1] = '\0';
	    temp.score = 0;
	    temp.socket = newsockfd;
	    temp.num_partite = 0;

        //chiamo la Find che controlla  se nella mia lista dei già registrati è già presente
        //attento che usa i PUNTATORI non variabili 
	    ItemType* found_player = Find(registered_players, temp);
        // se non lo trova lo inserisce nei registrati e nei connessi
	    if (found_player == NULL) {
	        registered_players = EnqueueLast(registered_players, temp);
	        connected_players = EnqueueLast(connected_players, temp);
	        printf("Nuovo giocatore registrato e connesso: %s\n", temp.name);
	    } 
        //se lo trovo imposto lui come gioatore 
        else{
	        found_player->socket = newsockfd;
            // lo trovo grazie al puntatore che mi ritorna la Find
	        connected_players = EnqueueLast(connected_players, *found_player);
	        printf("Giocatore già registrato e connesso: %s\n", found_player->name);
	    }

        // se ho i giocatori genero numeri e simulo
	    if (getLength(connected_players) >= MAX_PLAYERS) {
	        printf("\n--- Inizio simulazione partita ---\n");

	        int total_players = getLength(connected_players);
	        ItemType players[MAX_PLAYERS];
	        NODE* current = connected_players;
	        for (int i = 0; i < total_players && current != NULL; i++) {
	            players[i] = current->item;
	            current = current->next;
	        }


	        for (int i = 0; i < total_players; i++) {
	            int r = i + rand() % (total_players - i);
	            ItemType temp = players[i];
	            players[i] = players[r];
	            players[r] = temp;
	        }
            // punti da asseganre dopo in basse al nuovo ordine lista
	        int points[3] = {3, 2, 1};

	        for (int i = 0; i < total_players; i++) {
	            ItemType* reg_player = Find(registered_players, players[i]);
	            char message[BUF_SIZE];
                //se ho più di 3 giocatori al 4 e 5 do 0 punti
	            if (i < 3) 
                {
	                if (reg_player != NULL) {
	                    reg_player->score += points[i];
	                    reg_player->num_partite++;
	                }
	                sprintf(message,"Partita terminata!\nClassifica: %d° posto.\nHai ricevuto %d punti.\nPunteggio totale: %.2f\nPartite giocate: %d\n",
	                        i + 1, points[i], reg_player->score, reg_player->num_partite);
	            } 
                else 
                {
	                if (reg_player != NULL) {
	                    reg_player->num_partite++;
	                }
	                sprintf(message,
	                        "Partita terminata!\nClassifica: %d° posto.\nHai perso e ricevuto 0 punti.\nPartite giocate: %d\n",
	                        i + 1, reg_player->num_partite);
	            }
                //invio tutti i risultati ai client 
	            send(players[i].socket, message, strlen(message), 0);
	        }

	        printf("--- Fine simulazione partita ---\n\n");

            //ora voglio stampare totale dei giocatori che sono registarti
            //provato compareitem ma mi da core dumped (?)
            PrintList(registered_players);

	        current = connected_players;
	        while (current != NULL) {
	            close(current->item.socket);
	            current = current->next;
	        }
            //pulisco lista per nuova partita 
	        connected_players = DeleteList(connected_players);
	    }
	}


     return 0;
 }