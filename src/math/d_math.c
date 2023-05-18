#include "d_math.h"
#include <math.h>
#include "../camera/camera.h"

/* idk how this works, but it works pretty well ig */
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

/* Returns a & b of y = a*b^x for exponential equation */
void gen_exponential_data(double c[5][2], struct calibration_data *cdata)
{
	uint8_t n = 5; /* no. of points */

	double log_c[5][2];

	/* initiliaze our log of y array for linear regression */
	for(int i = 0; i < n; i++)
	{
		log_c[i][0] = c[i][0];
		log_c[i][1] = log10(c[i][1]);
	}

	/* find our sums needed for equation */
	double xy_sum        = 0;
	double x_sum	     = 0;
	double y_sum 	     = 0;
	double x_squared_sum = 0;

	for(int i = 0; i < n; i++)
	{
		xy_sum        += (log_c[i][0] * log_c[i][1]);
		x_sum         += (log_c[i][0]);
		y_sum         += (log_c[i][1]);
		x_squared_sum += (pow(log_c[i][0],2));
	}
	
	/* do our linear regression */
	double lm, lb, div_comp;

	/* instead of doing this twice we do it once */
	div_comp = ((n*x_squared_sum) - pow(x_sum,2));
	
	lm = ((y_sum*x_squared_sum) - (x_sum * xy_sum)) / div_comp;
	lb = ((n*xy_sum)-(x_sum*y_sum)) / div_comp;

	/* set our data points */
	cdata->a = pow(10,lm);
	cdata->b = pow(10,lb);
}

/* do our actual exponential equation */
float distance_exponential(double area, struct calibration_data cdata)
{
	return(cdata.a*pow(cdata.b,area));
}

