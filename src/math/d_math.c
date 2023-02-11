#include "d_math.h"
int grab_distance(double p[4][2], struct calibration_data *cdata)
{
	/* Grab left side distance */
	int cur_l_length = (p[2][1] - p[1][1]);
	
	/* Grab right side distance */
	int cur_r_length = (p[3][1] - p[0][1]);

	float per_l_change = cur_l_length / cdata->l_length;
	float per_r_change = cur_r_length / cdata->r_length;

	/* i so smart haha */

	float average_change = (per_l_change + per_r_change)/2;
	
	return (int)(average_change * cdata->distance);
}

int check_angle(int cur_l_length, int cur_r_length)
{
	if(cur_l_length > cur_r_length)
	{
		return 1; /* Tilted left */
	}
	else
	{
		return 2; /* Tilted right */
	}
}
