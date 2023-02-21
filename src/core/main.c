#include <apriltag/common/zarray.h>
#include <apriltag/apriltag.h>
#include <apriltag/tag16h5.h>
#include <apriltag/common/image_u8.h>
#include "../camera/processing.h"
#include "../camera/camera.h"
#include "../camera/config.h"
#include "../math/d_math.h"
#include "../server/server_client.h"
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <sys/time.h>
#include <pthread.h>
#define DECISION_THRESHOLD 70

int run = 1;
int debug = 0;

/* Handles Ctrl + C end of loop */
void intHandler(int useless)
{
	run = 0;	
}

int main(int argc, char *argv[])
{
	pthread_t thread;
	int iret1;

	/* Arg -d is debug */
	if(argc == 2)
	{
		if(0 == strcmp("-d",argv[1]))
		{
			debug = 1;
		}
	}


	/* Set calibration data */	
	struct calibration_data cdata;
	cdata.constant = 3725.19;
	cdata.fov_radians = 0.9337511;

	int stride = generate_stride(CAMERA_WIDTH, 96);
	image_u8_t *im = create_image_u8(CAMERA_WIDTH,CAMERA_HEIGHT,stride);
	
	/* Default camera device is video0 */
	int fd = init_everything(CAMERA_WIDTH,CAMERA_HEIGHT);

	/* Start server */
	iret1 = pthread_create(&thread,NULL,thread_function,NULL);

	start_stream(fd);

	apriltag_detector_t *td = apriltag_detector_create();
	apriltag_family_t *tf = tag16h5_create();
	apriltag_detector_add_family(td,tf);

	/* TODO: have default set location for camera setting! */
	set_settings_from_config("../cfg/test.camcfg",td);

	/* Handles Ctrl + C end of loop */
	signal(SIGINT, intHandler);
	apriltag_detection_t *det;

	if(debug)
	{
		/* Prints out test images */
		run = 0;
		td->debug = 1;
		get_detections(td,im);
	}

	while(run)
	{
		zarray_t* detections = get_detections(td,im);
		if(zarray_size(detections) != 0)
		{
			for(int i = 0; i < zarray_size(detections); i++)
			{
				zarray_get(detections, i, &det);

				if(det->decision_margin > DECISION_THRESHOLD && det->id >= 1 && det->id <= 8)
				{
					printf("FOUND! ID: %i D_M: %f\n",det->id, det->decision_margin);
					printf("C: X%f Y%f \n",det->c[0],det->c[1]);
					printf("AREA: %i\n",grab_area(det->p));
					float dist = grab_distance(det->p,&cdata);
					update_packet(det->id,dist,grab_angle(dist,det->c[0],&cdata));
				
				}
			}
		
		}
		zarray_destroy(detections);
	}
	close_cam(fd);

	tag16h5_destroy(tf);
	apriltag_detector_destroy(td);
	return 0;
}
