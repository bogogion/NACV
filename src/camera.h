#include <errno.h>
#include <stdlib.h>
#include "../lib/libv4l/include/libv4l2.h"

/* Apriltag detections! */
#include <apriltag/common/zarray.h>
#include <apriltag/apriltag.h>
#include <apriltag/common/image_u8.h>


#define CLEAR(x) memset(&(x), 0, sizeof(x))

/* There will be multiple of these */
struct buffer {
	void      *start;
	size_t    length;
};

/* Ease of use function, used for safe v4l2_ calls */
void xioctl(int fh, int request, void *arg);
zarray_t * get_detections(apriltag_detector_t *td,image_u8_t *im); /* Reminder; make sure to free the zarray_t generated*/
int init_everything(int width, int height, char *dev_name);
void init_mmap(); 
void set_cam_settings(int width, int height, int pformat); /* Used internally in init_cam */
int init_cam(char *dev_name, int width, int height); /* Return file descriptor */
void start_stream(int fd);
void close_cam(int fd); /* Cleans buffer too */
