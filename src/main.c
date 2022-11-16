#include <apriltag/common/zarray.h>
#include <apriltag/apriltag.h>
#include <apriltag/tag36h11.h>
#include <apriltag/common/image_u8.h>

#include <stdio.h>
#include <stdlib.h>

#include "main.h"
int main()
{
	image_u8_t* im = image_u8_create_from_pnm("test.pgm");
	apriltag_detector_t *td = apriltag_detector_create();
	apriltag_family_t *tf = tag36h11_create();
	apriltag_detector_add_family(td,tf);
	zarray_t *detections = apriltag_detector_detect(td,im);
	calibration_data_t cdata;
	for(int i = 0; i < zarray_size(detections); i++)
	{
		apriltag_detection_t *det;
		zarray_get(detections,i,&det);
		printf("DETECTION: %i\n",i+1);
		printf("LOCATION: %f,%f\n",det->c[0],det->c[1]);
		printf("DECISION_M: %f\n",det->decision_margin);
		printf("HAMMING: %i\n",det->hamming);
		printf("ID: %i\n",det->id);
		/* Corners, useful for distance calculations */
		printf("CORNER LOCATIONS: \n");
		printf("C1: %f,%f\n", det->p[0][0],det->p[0][1]);
		printf("C2: %f,%f\n", det->p[1][0],det->p[1][1]);
		printf("C3: %f,%f\n", det->p[2][0],det->p[2][1]);
		printf("C4: %f,%f\n", det->p[3][0],det->p[3][1]);
	
		printf("L1: %f\n",calc_distance(det->p[0][0],det->p[0][1],det->p[1][0],det->p[1][1]));
		printf("L2: %f\n",calc_distance(det->p[0][0],det->p[0][1],det->p[3][0],det->p[3][1]));
		printf("AREA: %f\n",calc_area(det->p));
	}
	tag36h11_destroy(tf);
	apriltag_detector_destroy(td);
	return 0;
}
