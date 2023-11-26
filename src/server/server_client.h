#include <stddef.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include "../core/shared.h"
#include "s_utils.h"

#ifndef SERVER_H
#define SERVER_H

#define MAX_MES_SIZE sizeof(struct aprildata)

//void *thread_function(void *vargp);
void init_server(char *address, int port);
void send_message(struct aprildata *data); /* Sends message to client */
#endif
