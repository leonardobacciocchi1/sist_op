#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netdb.h>


#define BUF_SIZE 1000


char *host_name = "127.0.0.1"; /* local host */
int port = 8080;


int main(int argc, char *argv[]) 
{
	struct sockaddr_in serv_addr;
 	struct hostent* server;	
	int num,numminimo, answer;
    char name[20],  bufferinv[256];
	
	if (argc != 4) { 
		printf("Error: %s integer\n", argv[0]);
		exit(-1);
	}

	num = atoi(argv[2]);
    numminimo = atoi(argv[3]);
    strcpy(name, argv[1]);
    
	
    //printf("Usage: nome = %s, numero = %d\n", name, num);
		
	
	if ( ( server = gethostbyname(host_name) ) == 0 ) 
	{
		perror("Error resolving local host\n");
		exit(1);
	}

	bzero(&serv_addr, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = ((struct in_addr *)(server->h_addr_list[0]))->s_addr;
	serv_addr.sin_port = htons(port);
	char buffer[BUF_SIZE];
	int sockfd = socket( PF_INET, SOCK_STREAM, 0 );
	if ( sockfd == -1 ) 
	{
		perror("Error opening socket\n");
		exit(1);
	}    

	if ( connect(sockfd, (void*)&serv_addr, sizeof(serv_addr) ) == -1 ) 
	{
		perror("Error connecting to socket\n");
		exit(1);
	}


	printf("Send the data: NAME: %s, NUMERO DISPONIBILI: %d, QUANTITÀ MINIMA %d\n", name, num, numminimo);
    snprintf(bufferinv, sizeof(bufferinv), "%s %d %d", name, num, numminimo);
	/* This sends the string plus the string terminator '\0' */
	if ( send(sockfd, bufferinv, strlen(bufferinv), 0) == -1 ) 
	{
		perror("Error on send\n");
		exit(1);
	}

	printf("waiting response \n");
	
	recv(sockfd, buffer, sizeof(buffer), 0);
	
	char *token = strtok(buffer, " ");
	char nomericevuto[30];
	strncpy(nomericevuto, token, sizeof(nomericevuto));
	int val;
	token = strtok(NULL, " ");
	val= atoi(token);
	//sscanf(buffer, "%s %d", nomericevuto,val);
	printf("Rifornisco centro: Nome: %s Socket: %d \n",nomericevuto, val);

	close(sockfd);

	return 0;
}


