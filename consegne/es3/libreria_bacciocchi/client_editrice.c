#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080
#define SERVER_IP "127.0.0.1"

int main() {
    int sock;
    struct sockaddr_in server_addr;
    char name[20], num[20], buffer[256], bufferinv[256];
    //char buffer1[256];

    // Creazione socket
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == -1) {
        perror("Errore nella creazione della socket");
        exit(EXIT_FAILURE);
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = inet_addr(SERVER_IP);

    // Connessione al server
    if (connect(sock, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
        perror("Errore nella connessione al server");
        exit(EXIT_FAILURE);
    }

    // Inserimento nome giocatore
    printf("Username: ");
    //prendo nome client con cui mi loggo, meglio di scanf perchè prende intera stringa.
    fgets(name, sizeof(name), stdin);
    // Rimuove il carattere di nuova riga dall'username
    name[strcspn(name, "\n")] = 0;  
	printf("numero copie: ");
	fgets(num, sizeof(num), stdin);
	num[strcspn(num, "\n")] = 0;

	snprintf(bufferinv, sizeof(buffer), "%s %s", name, num);
    // Invio nome al server
    send(sock, bufferinv, strlen(bufferinv), 0);


    // Chiusura socket
    close(sock);
    return 0;
}