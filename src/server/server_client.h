#include <stddef.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include "../core/shared.h"
#include "s_utils.h"

#ifndef SERVER_H
#define SERVER_H

#define MAX_MES_SIZE sizeof(struct aprildata)

// TODO: use struct instead 
/* These macros affect everything, make sure to change these if you need to change port/ip */
#define PORT 5805
#define C_PORT 5805
#define IP "127.0.0.1"

void *thread_function(void *vargp);
void init_server(char *address, int port);
void send_message(struct aprildata *data, int sock_fd, struct sockaddr_in server_address, struct sockaddr_in client_address); /* Sends message to client */
#endif
