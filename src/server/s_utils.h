#include <stdint.h>
#ifndef NACV_NET_UTILS
#define NACV_NET_UTILS

/* Defintion for server information for both the controller and data stream */

typedef struct server_info
{
	uint32_t camera_id;
	char ip[11];
	int team_number;
	uint16_t port;
} s_info;

void create_valid_server_info(s_info *info, int team_number, uint16_t port, uint32_t camera_id);
void team_number_to_str(int team_number, char buf[11]);
uint32_t flip_u32(uint32_t no);
float flip_f32(float no);
#endif
