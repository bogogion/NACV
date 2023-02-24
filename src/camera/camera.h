#include <errno.h>
#include <stdlib.h>
#include "../../lib/libv4l/include/libv4l2.h"

#define CAMERA_WIDTH    640
#define CAMERA_HEIGHT   480

#define ID_CLASS_USER   0x980000
#define ID_CLASS_CAMERA 0x9a0000

/* Apriltag detections! */
#include <apriltag/common/zarray.h>
#include <apriltag/apriltag.h>
#include <apriltag/common/image_u8.h>

#define CLEAR(x) memset(&(x), 0, sizeof(x))

struct buffer {
	void      *start;
	size_t    length;
};

/* Ease of use function, used for safe v4l2_ calls */
void test_input(image_u8_t *im);
void set_camera_settings(uint32_t ctrl_class, uint32_t id, int32_t value); /* EXT Call */
void set_user_setting(uint32_t id, int32_t value);
void xioctl(int fh, int request, void *arg);
zarray_t * get_detections(apriltag_detector_t *td,image_u8_t *im); /* Reminder; make sure to free the zarray_t generated*/
int init_everything(int width, int height);
void init_mmap(); 
void set_cam_settings(int width, int height, int pformat); /* Used internally in init_cam */
int init_cam(char *dev_name, int width, int height); /* Return file descriptor */
void start_stream(int fd);
void close_cam(int fd); /* Cleans buffer too */
