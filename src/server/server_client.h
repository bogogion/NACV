#include <stddef.h>
#include <netinet/in.h>
#include <sys/socket.h>

#ifndef SERVER_H
#define SERVER_H

#define MAX_MES_SIZE 9

/* These macros affect everything, make sure to change these if you need to change port/ip */
#define PORT 5805
#define C_PORT 5805
#define IP "127.0.0.1"
#define C_IP "127.0.0.1"

/* Current send mode */
union float_conv
{
	float   f;
	char s[4];
};

typedef struct server_packet
{
	unsigned char    id;
	union float_conv conv;
	float            angle;
	float            dist;
} s_packet;

void update_packet(int id, float dist, float angle);
void *thread_function(void *vargp);
void init_server(char *address, int port);
void send_message(s_packet packet, int sock_fd, struct sockaddr_in server_address, struct sockaddr_in client_address); /* Sends message to client */
void update_shm(int fd, int size);
#endif
