#include <stdlib.h>
#include <stdio.h>
#include <math.h>

/* Calc distance between two pixel locations
   for apriltag: use corners use 
   p[0][0-1] p[1][0-1] for the first line (top line of cube) 
   p[0][0-1] p[3][0-1] for the second line (left side of the cube) */
double calc_distance(double x1, double y1, double x2, double y2)
{
	/* (X2-X1)^2+(Y2+Y1)^2 squared */

	double x = (x2-x1) * (x2-x1);
	double y = (y2-y1) * (y2-y1);
	
	double squared_root = sqrt(x+y);
	return squared_root;
}

/* Combined code with function above. 
   arg p should come from apriltag_detector_t struct, in code 'det->p' 
   When getting the area in code, use below function and only use above function for
   specific use cases
   */
double calc_area(double p[4][2])
{
	double top_dist;
	double left_dist;

	top_dist = calc_distance(p[0][0],p[0][1],p[1][0],p[1][1]);
	left_dist = calc_distance(p[0][0],p[0][1],p[3][0],p[3][1]);

	return (top_dist * left_dist);
}
/* Load the area of the calibrated square. This will be used for distance estimation. */
double load_calibration_from_file(char *filename)
{
	/* Make sure to pass in file that has read. */
	FILE *file; 
	file = fopen(filename,"r+");
	
	char line[25];
	while(fgets(line,25,file))
	{
		return atof(line);
	}
	fclose(file);
	return -1;	
}

void save_calibration_data(char *filename, double cal)
{
	/* Make sure to pass in file that has write. */
	FILE *file;
	file = fopen(filename,"w");
	fprintf(file,"%f",cal);
	fclose(file);
}
