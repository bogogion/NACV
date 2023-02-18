#include "server_client.h"
#include <string.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <unistd.h>

char buf[MAX_MES_SIZE];

void init_server(char *address, int port)
{}

void send_message(s_packet packet, int sock_fd)
{
	/* Setup packet */
	buf[0] = packet.id;
	
	/* Distance float */
	packet.conv.f = packet.dist;

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

	/* Send packet */
	if(send(sock_fd, buf, MAX_MES_SIZE,0) == -1)
	{
		printf("client seems to be the antichrist, message didn't send");
	}
}
