/* Distance math */

/* Data from calibration */
struct calibration_data
{
	/* Constants */
	float constant;
	float fov_radians; /* Fov angle to radians */
};
float grab_angle(double p[4][2]);
int grab_area(double p[4][2]);
float grab_distance(double p[4][2], struct calibration_data *cdata);
