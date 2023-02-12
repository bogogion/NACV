#include "d_math.h"
#include <math.h>

/* x = center x 
 * dt = distance
 * */

float grab_angle(float dt, int x, struct calibration_data *cdata)
{
	/* c_ camera variable */
	
	double c_fov_distance;
	double angle_to_target;
	double c_distance_to_target;

	/* distance to point * tan(radians/2)*/
	c_fov_distance = dt * tan(cdata->fov_radians/2);
	c_distance_to_target = (x/320)*c_fov_distance;
	
	angle_to_target = atan(c_fov_distance/dt);

	/* Convert radians to angle */
	angle_to_target = (angle_to_target/3.12)*180;
	return angle_to_target;
}

int grab_distance(double p[4][2], struct calibration_data *cdata)
{
	int area = ((p[0][0]*p[1][1])-(p[1][0]*p[0][1])) + ((p[1][0]*p[2][1])-(p[2][0]*p[1][1]))
		   + ((p[2][0]*p[3][1])-(p[3][0]*p[2][1])) + ((p[3][0]*p[0][1])-(p[0][0]*p[3][1]));
	area = area /2;
	
	/* Absolute value*/
	if(area < 0)
	{
		area = area * -1;
	}

	return (cdata->constant * pow(area,-.5));
}

int grab_area(double p[4][2])
{	
	int area = ((p[0][0]*p[1][1])-(p[1][0]*p[0][1])) + ((p[1][0]*p[2][1])-(p[2][0]*p[1][1]))
		   + ((p[2][0]*p[3][1])-(p[3][0]*p[2][1])) + ((p[3][0]*p[0][1])-(p[0][0]*p[3][1]));
	area = area /2;

	if(area < 0)
	{
		return area*-1;
	}
	return area;
}
