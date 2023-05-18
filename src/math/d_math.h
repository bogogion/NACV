/* Distance math */

#ifndef MATH_H
#define MATH_H
/* Data from calibration */
struct calibration_data
{
	/* Exponential constants */
	float a,b;
};

float grab_angle(double p[4][2]);
int grab_area(double p[4][2]);
void gen_exponential_data(double c[5][2], struct calibration_data *cdata);

/* Calibration functions */
float distance_exponential(double area, struct calibration_data cdata);
#endif
