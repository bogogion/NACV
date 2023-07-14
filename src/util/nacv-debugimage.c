/* Simple utility to print debug AprilTag images */
#include <string.h>
#include <stdio.h>

#include "../core/nacv.h"

int main(int argc, char *argv[])
{
	int fd = init_everything(CAMERA_WIDTH, CAMERA_HEIGHT);

	struct apriltag_stack astack;
	create_apriltag_stack(&astack, CAMERA_WIDTH, CAMERA_HEIGHT);

	astack.td->quad_decimate = 2.0;
	astack.td->debug = 1;

	start_stream(fd);

	apriltag_debug(astack);

	close_cam(fd);
	clean_apriltag_stack(&astack);
	return 0;
}
