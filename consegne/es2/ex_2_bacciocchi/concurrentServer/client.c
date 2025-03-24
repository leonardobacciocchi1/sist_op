#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include <netinet/in.h>

//definisco i parametri per fare la connessione 
#define BUF_SIZE 1000
#define PORT 8000
//in particolare qua definisco il numero di figli che voglio creare 
#define NUM_CLIENTS 5  
char *host_name = "127.0.0.1";

//funzione normalissima per creare numero pseudo-univoco
int generate_unique_number(int pid){
	srand(time(NULL) + pid);
	return rand();
}
//se non metto i puntatori non funziona
void *client_thread(void *arg) {
    int sockfd, num, answer;
	//meglio usare struct almeno basta copia incolla ogni es, nel caso cambio i parametri
    struct sockaddr_in serv_addr;
    int client_id = *(int *)arg;

    // Creazione della socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("Errore nella creazione del socket");
        pthread_exit(NULL);
    }

    // Impostazione dell'indirizzo del server
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr == host_name;
    serv_addr.sin_port = htons(PORT);

    // check connessione al server
    if (connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        perror("Errore nella connessione al server");
        pthread_exit(NULL);
    }

    // invio al server
	// usa l'ID del client, per inviare numeri diversi con time e rand 
    num = generate_unique_number(client_id);  
    printf("Client %d invia il numero: %d\n",client_id , num);
    if (send(sockfd, &num, sizeof(num), 0) < 0) {
        perror("Errore nell'invio al server");
        close(sockfd);
        pthread_exit(NULL);
    }

    // check ricezione risposta dal server
    if (recv(sockfd, &answer, sizeof(answer), 0) < 0) {
        perror("Errore nel ricevere la risposta dal server");
        close(sockfd);
        pthread_exit(NULL);
    }
    printf("Client %d ha ricevuto la risposta: %d\n", client_id, answer);

    // chiudi
    close(sockfd);
    pthread_exit(NULL);
}

int main() {
	// array di thread
    pthread_t threads[NUM_CLIENTS];  
	// array per identificare i client
    int client_ids[NUM_CLIENTS];  

    // avvia i thread, thread separato per ogni client 
    for (int i = 0; i < NUM_CLIENTS; i++) {
        client_ids[i] = i;
        if (pthread_create(&threads[i], NULL, client_thread, &client_ids[i]) != 0) {
            perror("Errore nella creazione del thread");
            exit(1);
        }
    }

    // aspetto che tutti i thread finiscano, copia incolla per ogni es 
    for (int i = 0; i < NUM_CLIENTS; i++) {
        pthread_join(threads[i], NULL);
    }

    return 0;
}
