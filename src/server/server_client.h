#include <stddef.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include "../core/shared.h"

#ifndef SERVER_H
#define SERVER_H

#define MAX_MES_SIZE sizeof(struct aprildata) + 4

/* These macros affect everything, make sure to change these if you need to change port/ip */
#define PORT 5805
#define C_PORT 5805
#define IP "127.0.0.1"

/* Current send mode */
union float_conv
{
	float   f;
	char s[4];
};

typedef struct server_packet
{
	uint32_t camera_id; /* Individual ID of the camera as defined by the user */
	void     *data;     /* Pointer to memory where aprildata resides          */
} s_packet;

void update_packet(void *addr);
void *thread_function(void *vargp);
void init_server(char *address, int port);
void send_message(s_packet packet, int sock_fd, struct sockaddr_in server_address, struct sockaddr_in client_address); /* Sends message to client */
#endif
