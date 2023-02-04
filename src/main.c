#include <apriltag/common/zarray.h>
#include <apriltag/apriltag.h>
#include <apriltag/tag16h5.h>
#include <apriltag/common/image_u8.h>
#include <apriltag/common/matd.h>
#include <apriltag/common/math_util.h>
#include "processing.h"
#include "camera.h"
#include "config.h"
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <sys/time.h>

#define DECISION_THRESHOLD 70

int run = 1;

/* Handles Ctrl + C end of loop */
void intHandler(int useless)
{
	run = 0;	
}

int main()
{
	int stride = generate_stride(CAMERA_WIDTH, 96);
	image_u8_t *im = create_image_u8(CAMERA_WIDTH,CAMERA_HEIGHT,stride);
	
	/* Default camera device is video0*/
	int fd = init_everything(CAMERA_WIDTH,CAMERA_HEIGHT);

	start_stream(fd);

	apriltag_detector_t *td = apriltag_detector_create();
	apriltag_family_t *tf = tag16h5_create();
	apriltag_detector_add_family(td,tf);

	/* TODO: have default set location for camera setting! */
	set_settings_from_config("../cfg/test.camcfg",td);

	signal(SIGINT, intHandler);
	apriltag_detection_t *det;
	while(run)
	{
		zarray_t* detections = get_detections(td,im);

		if(zarray_size(detections) != 0)
		{
			for(int i = 0; i < zarray_size(detections); i++)
			{
				zarray_get(detections, i, &det);

				if(det->decision_margin > DECISION_THRESHOLD && det->id >= 4 && det->id <= 6)
				{
					printf("FOUND! ID: %i D_M: %f\n",det->id, det->decision_margin);
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
