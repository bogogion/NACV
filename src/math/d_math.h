/* Distance math */

/* Data from calibration */
struct calibration_data
{
	/* Lenght in pixels of the left side and right side */
	int l_length;
	int r_length;
	
	/* In inches */
	int distance;
};

int grab_distance(double p[4][2], struct calibration_data *cdata);
int check_angle(int cur_l_length, int cur_r_length);
