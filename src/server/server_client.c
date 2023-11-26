#include "server_client.h"
#include "../core/shared.h"
#include <string.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <unistd.h>
#include <pthread.h>
#include <arpa/inet.h>
#include <sys/mman.h>
#include <sys/stat.h>

/* NOTE: this is SO fucked man god why don't i just fix it instead of complaining lol */
/* the mutant servercode uhhh mane i got no idea what is even going on but like you know lol hahaha :D :D :D */
int server_fd, new_socket;
struct sockaddr_in server_address, client_address;

/* void *thread_function(void *vargp)
{
	init_server(IP,PORT);
} */
/* Really more of a client anyways... TODO: rework this so it's client named and focused */
/* TODO: zane please, for the love of god, fix this! 11/26 */
/* Literally talking to myself rn... i thin k i summorned satan */
void init_server(char *address, int port)
{
	int addrlen = sizeof(server_address);

	server_fd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

	/* Fill adress structures */
	server_address.sin_family = AF_INET;
	inet_pton(AF_INET, address, &server_address.sin_addr);
	server_address.sin_port = htons(port);

	//bind(server_fd, (struct sockaddr*)&server_address,sizeof(server_address));
}

/* No need to pass extra data since it's alreayd known and saved */
void send_message(struct aprildata *data)
{
	/* Send packet */
	if(sendto(server_fd,(const char*)data,MAX_MES_SIZE,0,(struct sockaddr*)&server_address,sizeof(server_address)) == -1)
	{
		printf("client seems to be the antichrist, message didn't send");
	}
}
