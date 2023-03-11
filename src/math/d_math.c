#include "d_math.h"
#include <math.h>
#include "../camera/camera.h"

/* x = center x 
 * dt = distance
 * */

float grab_angle(double p[4][2])
{
	float dx,dy;
	dx = p[1][0]-p[0][0];
	dy = p[0][1]-p[3][1];

	/* -45 is the constant */
	float angle_to_target = (((atan(dx/dy))/3.12)*180)-45.0;
	if(angle_to_target<0){return (angle_to_target*-1);}
	
	return angle_to_target;
}

float grab_distance(double p[4][2], struct calibration_data *cdata)
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

/* debug function */
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
