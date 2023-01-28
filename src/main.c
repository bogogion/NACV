#include <apriltag/common/zarray.h>
#include <apriltag/apriltag.h>
#include <apriltag/tag16h5.h>
#include <apriltag/common/image_u8.h>
#include <apriltag/common/matd.h>
#include <apriltag/common/math_util.h>
#include "processing.h"
#include "camera.h"
#include <stdio.h>
#include <stdlib.h>
#include "main.h"
#include <unistd.h>
#include <sys/time.h>
#include <sys/types.h>

void main_loop(image_u8_t *im, struct buffer *buffers, apriltag_detector_t *td, int stride)
{
	int fd = init_everything(640,480, "/dev/video0", buffers);
	fd_set fds;
	struct timeval tv;
	/* Start stream! */
	start_stream(fd);
	
	/* Main loop, handles detections! */

	uint32_t buflen = ((640,480)*3);
	uint8_t *buffer = malloc(buflen);
	
	printf("Created buffers\n");

	int *info;

	while(1)
	{
		info = grab_frame();
		printf("Before memcpy %i %i \n",info[0],info[1]);
		memcpy(buffer, buffers[info[0]].start, info[1]);
		printf("After memcpy\n");
		convert_rgb24_proper(640,480,stride,buffer,im);

		zarray_t *detections = apriltag_detector_detect(td,im);
		printf("DETECTIONS: %i\n", zarray_size(detections));
		requeue(); /* Continue */
	}
}



int main()
{
	
	int stride = generate_stride(640, DEFAULT_ALIGNMENT_U8);

	image_u8_t *im = create_image_u8(640,480,stride);

	struct buffer *buffers; 

	apriltag_detector_t *td = apriltag_detector_create();
	apriltag_family_t *tf = tag16h5_create();
	apriltag_detector_add_family(td,tf);
	
	main_loop(im,buffers,td,stride);



	zarray_t *detections = apriltag_detector_detect(td,im);
	calibration_data_t cdata;
	printf("%i\n", zarray_size(detections));
	for(int i = 0; i < zarray_size(detections); i++)
	{
		apriltag_detection_t *det;
		zarray_get(detections,i,&det);

		/* False-positive detection */

		if(det->decision_margin < 10)
		{
			continue;
		}
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
	
		/* matd_t *H */
		printf("nrows: %i\n", det->H->nrows);
		printf("ncols: %i\n", det->H->ncols);
		
		double *data = det->H->data;
		
		for(int i = 0; i < det->H->nrows;i++)
		{
			for(int j = 0; j < det->H->ncols;j++)
			{
				printf("R:%i C:%i D:%f \n",i+1,j+1,data[(i*det->H->ncols) + j]);
			}
		}



		printf("L1: %f\n",calc_distance(det->p[0][0],det->p[0][1],det->p[1][0],det->p[1][1]));
		printf("L2: %f\n",calc_distance(det->p[0][0],det->p[0][1],det->p[3][0],det->p[3][1]));
		printf("AREA: %f\n",calc_area(det->p));
		cdata.ft = 3;
		cdata.area = calc_area(det->p);
		printf("DIST %fft, %fin\n",calc_distance_tags(&cdata,200000),calc_distance_tags(&cdata,200000)*12);
		float res[2] = {1000,1000}; /* Test res, should be res of camera */
		float off[2] = {0}; /*Initalize the storage variable*/
		local_offset(det->c,res,&off);
		printf("OFFSET: %f,%f\n",off[0],off[1]);
	}
	tag36h11_destroy(tf);
	apriltag_detector_destroy(td);
	return 0;
}
