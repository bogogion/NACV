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

char buf[MAX_MES_SIZE];
s_packet G_packet;

uint8_t byte_order = 1;

int server_fd, new_socket;
struct sockaddr_in server_address, client_address;

void update_packet(int id, float dist, float angle)
{
	G_packet.id = id;
	G_packet.dist = dist;
	G_packet.angle = angle;
	
	send_message(G_packet,server_fd,server_address,client_address);
}
void *thread_function(void *vargp)
{
	init_server(IP,PORT);
}
void init_server(char *address, int port)
{
	int addrlen = sizeof(server_address);

	server_fd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

	/* Fill adress structures */
	server_address.sin_family = AF_INET;
	inet_pton(AF_INET, address, &server_address.sin_addr);
	server_address.sin_port = htons(port);

	client_address.sin_family = AF_INET;
	inet_pton(AF_INET, C_IP, &client_address.sin_addr);
	client_address.sin_port = htons(C_PORT);

	bind(server_fd, (struct sockaddr*)&server_address,sizeof(server_address));

	/* search for endianness */
	uint8_t swap[2] = {1,0}; /* 00000001 00000000 on big endian */ 
	if(*(uint16_t *)swap == 1){byte_order = 0;}
}

void send_message(s_packet packet, int sock_fd, struct sockaddr_in server_address, struct sockaddr_in client_address)
{
	/* Setup packet */
	buf[0] = packet.id;
	
	/* Distance float */
	packet.conv.f = packet.dist;


	/* byte order checking */
	if(!byte_order)
	{
		buf[1] = packet.conv.s[3];
		buf[2] = packet.conv.s[2];
		buf[3] = packet.conv.s[1];
		buf[4] = packet.conv.s[0];

		/* Angle float */
		packet.conv.f = packet.angle;

		buf[5] = packet.conv.s[3];
		buf[6] = packet.conv.s[2];
		buf[7] = packet.conv.s[1];
		buf[8] = packet.conv.s[0];
	} else {		
		buf[1] = packet.conv.s[0];
		buf[2] = packet.conv.s[1];
		buf[3] = packet.conv.s[2];
		buf[4] = packet.conv.s[3];

		/* Angle float */
		packet.conv.f = packet.angle;

		buf[5] = packet.conv.s[0];
		buf[6] = packet.conv.s[1];
		buf[7] = packet.conv.s[2];
		buf[8] = packet.conv.s[3];
	}
	/* Send packet */
	if(sendto(sock_fd,buf,9,0,(struct sockaddr*)&client_address,sizeof(server_address)) == -1)
	{
		printf("client seems to be the antichrist, message didn't send");
	}
}
