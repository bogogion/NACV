#include <errno.h>
#include <stdlib.h>
#include "../../lib/libv4l/include/libv4l2.h"
#include "../math/d_math.h"

#ifndef CAMERA_H
#define CAMERA_H

#define CAMERA_WIDTH    640
#define CAMERA_HEIGHT   480
#define SIZE CAMERA_HEIGHT * CAMERA_WIDTH * 3

#define ID_CLASS_USER   0x980000
#define ID_CLASS_CAMERA 0x9a0000

#define DECISION_THRESHOLD 70

#define CLEAR(x) memset(&(x), 0, sizeof(x))

struct buffer {
	void      *start;
	size_t    length;
};

int check_for_sensormode(); /* returns true or false if sensor mode is supported (if the modified kernel is used!) */
void init_shm(unsigned int buffer_size);
void cleanup_shm(unsigned int buffer_size);
void set_camera_settings(uint32_t ctrl_class, uint32_t id, int32_t value); /* EXT Call */
void xioctl(int fh, int request, void *arg);
void mainloop_shm();
int init_everything(int width, int height);
void set_cam_settings(int width, int height, int pformat); /* Used internally in init_cam */
int init_cam(char *dev_name, int width, int height); /* Return file descriptor */
void start_stream(int fd);
void close_cam(int fd); /* Cleans buffer too */
#endif
