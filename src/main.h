#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
/*TODO: get test images for 16h5, which will be the new standard, ty so much for changing it at random FRC! */
#include <apriltag/common/zarray.h>
#include <apriltag/apriltag.h>
#include <apriltag/tag16h5.h>
#include <apriltag/common/image_u8.h>
#include <apriltag/tag36h11.h>

/* Macros */
#define MAX_THREADS 4
#define RESX 1000 /* Test values, change to actual camera resolution */
#define RESY 1000 
typedef struct calibration_data
{
	float ft; /* Distance from camera AprilTag is situated */
	float area; /* Area in pixels of the AprilTag at this distance */
} calibration_data_t;
/*
 * c[2] = det->c (from apriltag detection)
 * res[2] = resolution in pixels, X,Y
 * store = a two element float array to store the data in, this MUST be passed in or it will break!
 * */

apriltag_detector_t * init_detector()
{
	apriltag_detector_t *td = apriltag_detector_create();
	td->nthreads = MAX_THREADS;
	return(td);
}

void local_offset(double c[2], float res[2], float *store)
{
	float x = c[0];
	float y = c[1];

	float max_x = res[0];
	float max_y = res[1];

	float x_offset;
	float y_offset;
	

	/* Left is anything less than X * 1/2 */
	/* Right is anything more than X * 1/2 */
	if(x < (max_x * .5))
	{
		x_offset = (x - (max_x * .5));	
	} else if(c[0] > (x * .5))
	{
		x_offset = (x - (max_x * .5));
	}
	/* Up is anything below Y * 1/2 */
	/* Down is anything above Y * 1/2 */
	if(c[1] < (y * .5))
	{
		y_offset = (y - (max_y * .5));
	} else if(c[1])
	{
		y_offset = (y - (max_y * .5));
	}

	store[0]=x_offset;
	store[1]=y_offset;
	/* No need to return since the pointer is changed. */
}

/* Returns distance in ft from target 
   Not insanely accurate, however for the purposes of this project
   this will work perfectly.
   Only limitation is calibration needs!
*/
float calc_distance_tags(calibration_data_t *cdata, float new_area)
{
	float per_change = (cdata->area/new_area); /* Simply percent change */
	return(cdata->ft*per_change);
}

/* Calc distance between two pixel locations
   for apriltag: use corners use 
   p[0][0-1] p[1][0-1] for the first line (top line of cube) 
   p[0][0-1] p[3][0-1] for the second line (left side of the cube) */
float calc_distance(double x1, double y1, double x2, double y2)
{
	/* (X2-X1)^2+(Y2+Y1)^2 squared */

	double x = (x2-x1) * (x2-x1);
	double y = (y2-y1) * (y2-y1);
	
	/* Variables casted to float in order to have a faster square root
	   This looses barely any accuracy (none that is important to distance calc, unless doing millimeters lol)
	   and is much quicker. 
	*/

	x = (float) x;
	y = (float) y;

	float squared_root = sqrtf(x+y);
	return squared_root;
}

/* Combined code with function above. 
   arg p should come from apriltag_detector_t struct, in code 'det->p' 
   When getting the area in code, use below function and only use above function for
   specific use cases
   */
float calc_area(double p[4][2])
{
	float top_dist;
	float left_dist;

	top_dist = calc_distance(p[0][0],p[0][1],p[1][0],p[1][1]);
	left_dist = calc_distance(p[0][0],p[0][1],p[3][0],p[3][1]);

	return (top_dist * left_dist);
}
/* Load the area of the calibrated square. This will be used for distance estimation. */
void load_calibration_from_file(char *filename, calibration_data_t *cdata)
{
	/* Make sure to pass in file that has read. */
	FILE *file; 
	file = fopen(filename,"r+");

	char buf[50];
	char line[50];
	fgets(line,50,file); /* Grabs only first line. */

	/* Look through formatted string and parse it. */
	int j = 0;
	for(int i = 0; i<(sizeof(line)/sizeof(line[0]));i++)
	{
		if(line[i] == '#')
		{
			cdata->ft = atof(buf);

			/* Reset variables*/
			memset(buf,0,sizeof(buf));
			j = 0;

			continue;
		}	
		buf[j] = line[i]; /* Copy char to bufffer */
	        j++;	
	}
	/* Last buffer value should be float */
	cdata->area = atof(buf);
}

void save_calibration_data(char *filename, calibration_data_t *cdata)
{
	/* Make sure to pass in file that has write. */
	FILE *file;
	file = fopen(filename,"w");
	fprintf(file,"%f#%f",cdata->ft,cdata->area);
	fclose(file);
}

/*
	Function to gen calibration struct from a static .pgm image, along with a cdata pointer
 	Use this function ONLY when the camera is a static FIXED position, as changes in position will render the
	calibration data null.

IMPORTANT: ONLY HAVE ON APRILTAG ON SCREEN AT ONE TIME FOR CALIBRATION
IMPORTANT: Default calibration distance is 3 feet, this is what by default the computer will use for distance calculations, 
	   so ensure it is 3 feet from camera. (pro tip: use a yard stick)
*/
void calibrate_from_image(char *image_addr, calibration_data_t *cdata)
{
	/* uhhh guuhh uguuhh guuuhhh uuhhhh urghh*/
	image_u8_t* im = image_u8_create_from_pnm(image_addr);
	/* Create detectors */	
	apriltag_detector_t *td = apriltag_detector_create();
	apriltag_family_t *tf = tag36h11_create();
	apriltag_detector_add_family(td,tf);
	/* Detect */
	zarray_t *detections = apriltag_detector_detect(td,im);
	apriltag_detection_t *det;
	/* Access detections */
	zarray_get(detections,0,&det);
	/* Assign to struct */
	cdata->ft = 3; /* Default 3ft for calibration, change this for further/closer */
	cdata->area = calc_area(det->p);
}
