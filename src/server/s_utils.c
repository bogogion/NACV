#include "s_utils.h"
#include <arpa/inet.h>

/* Converts team number into usable address to talk to RoboRIO */
void team_number_to_str(int team_number, char buf[11])
{
	/* 10.xx.xx.2 for RoboRIO */
	buf[0] = '1';
	buf[1] = '0';
	buf[2] = '.';

	/* Parse number into string */
	int x, y, z;
	x = team_number/1000;
	y = (team_number/100) % 10;
	z = ((team_number % 1000) / 10) % 10;

	buf[3] = x + 48;
	buf[4] = y + 48;
	buf[5] = '.';
	buf[6] = z + 48;
	buf[7] = (team_number - ((x * 1000) + (y * 100) + (z * 10))) + 48;
	buf[8] = '.';
	buf[9] = '2';
	buf[10] = '\0';
}

// TODO: remember why i was going to use this please
void create_valid_server_info(s_info *info, int team_number, uint16_t port, uint32_t camera_id)
{
	info->port = htons(port);
	info->team_number = team_number;
	info->camera_id = camera_id;

	team_number_to_str(team_number,info->ip);
}

uint32_t flip_u32(uint32_t no)
{
	uint32_t out;
	for(int i = 0; i < 4; i++)
	{
		((uint8_t *)&out)[i] = ((uint8_t *)&no)[3-i];
	}
	return out;
}

float flip_f32(float no)
{
	float out;
	for(int i = 0; i < 4; i++)
	{
		((uint8_t *)&out)[i] = ((uint8_t *)&no)[3-i];
	}
	return out;
}
