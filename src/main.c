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
#include <signal.h>
//#include "main.h"
#include <unistd.h>
#include <sys/time.h>
#include <sys/types.h>

#define DECISION_THRESHOLD 70
#define THREADS_NO 1

#define WIDTH 640
#define HEIGHT 480

int run = 1;


/* Handles Ctrl + C end of loop */
void intHandler(int useless)
{
	run = 0;	
}

int main()
{
	/* Timing */
	time_t start,end;

	int stride = generate_stride(WIDTH, 96);
	image_u8_t *im = create_image_u8(WIDTH,HEIGHT,stride);
	
	int fd = init_everything(WIDTH,HEIGHT,"/dev/video0");

	start_stream(fd);

	apriltag_detector_t *td = apriltag_detector_create();
	apriltag_family_t *tf = tag16h5_create();
	apriltag_detector_add_family(td,tf);
	
	td->nthreads = THREADS_NO;

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
	printf("%f\n", (end - start));

	close_cam(fd);

	tag16h5_destroy(tf);
	apriltag_detector_destroy(td);
	return 0;
}
