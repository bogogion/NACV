#include <stddef.h>
#define MAX_MES_SIZE 9

/* These macros affect everything, make sure to change these if you need to change port/ip */
#define PORT 3824
#define IP "127.0.0.1"

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
/* Function is called once main loop starts, handles multiple connections */
void init_server(char *address, int port);
void send_message(s_packet packet, int sock_fd); /* Sends message to client */
