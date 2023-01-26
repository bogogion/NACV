#include <errno.h>
#include <stdlib.h>
#include "../lib/libv4l/include/libv4l2.h"

#define CLEAR(x) memset(&(x), 0, sizeof(x))

/* There will be multiple of these */
struct buffer {
	void   *start;
	size_t length;
};

/* Ease of use function, used for safe v4l2_ calls */
void xioctl(int fh, int request, void *arg)
{
	int r;
	do {
		r = v4l2_ioctl(fh,request,arg);
	} while (r == -1 && ((errno == EINTR) || (errno == EAGAIN)));

	if (r == -1)
	{
		perror("XIOCTL_CALL");
		exit(EXIT_FAILURE);
	}

}

void init_mmap(struct buffer *buffers); /* Buffers param for safety */
void set_cam_settings(int width, int height, int pformat); /* Used internally in init_cam */
int init_cam(char *dev_name); /* Return file descriptor */
void start_stream(int fd);
void close_cam(int fd,struct buffer *buffers); /* Cleans buffer too */
