#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h> /* sleep */
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/mman.h>
#include <linux/videodev2.h>
#include "../lib/libv4l/include/libv4l2.h"
#include "camera.h"

int main()
{
	int fd = init_cam("/dev/video0");
	struct buffer *buffers;
	init_mmap(buffers);
	start_stream(fd);
	sleep(5);
	close_cam(fd,buffers);
	return 0; /* Remove in final product, used for test */
}

/* Declare variables */
/* Global for ease of use */
struct v4l2_format	    format;
struct v4l2_buffer	    v_buf;
struct v4l2_requestbuffers  v_request;
enum v4l2_buf_type	    v_type;
fd_set			    fds;
struct timeval		    tv;
int			    r, fd = -1;
unsigned int		    i, n_buffers;
/* char			    *dev_name = "/dev/video0"; */
/* struct buffer		    *buffers; */

/* Create a 2 buffer memory map of the device */
void init_mmap(struct buffer *buffers)
{
	CLEAR(v_request);
	v_request.count = 2;
	v_request.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	v_request.memory = V4L2_MEMORY_MMAP;
	xioctl(fd, VIDIOC_REQBUFS, &v_request);
	
	buffers = calloc(v_request.count, sizeof(*buffers));
	
	/* Initialize buffers and assign them */
	for(n_buffers = 0; n_buffers < v_request.count; n_buffers++)
	{
		CLEAR(v_buf);

		v_buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
		v_buf.memory = V4L2_MEMORY_MMAP;
		v_buf.index = n_buffers;
		
		xioctl(fd, VIDIOC_QUERYBUF, &v_buf);

		buffers[n_buffers].length = v_buf.length;
		buffers[n_buffers].start = v4l2_mmap(NULL, v_buf.length,
						     PROT_READ | PROT_WRITE,
						     MAP_SHARED, fd, v_buf.m.offset);
		if(MAP_FAILED == buffers[n_buffers].start)
		{
			perror("Memory Map Failure! ");
			exit(EXIT_FAILURE);
		}
	}
	
	for(i = 0; i < n_buffers; i++)
	{
		CLEAR(v_buf);
		v_buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;	
		v_buf.memory = V4L2_MEMORY_MMAP;
		v_buf.index = i;
	}
}

void set_cam_settings(int width, int height, int pformat)
{
	format.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	format.fmt.pix.width       = width;
	format.fmt.pix.height      = height;
	format.fmt.pix.pixelformat = pformat;
	format.fmt.pix.field       = V4L2_FIELD_INTERLACED;
	
	/* Force set settings */
	xioctl(fd, VIDIOC_S_FMT, &format);

	if(format.fmt.pix.pixelformat != pformat)
	{
		printf("FORMAT NOT SUPPORTED!");
		exit(EXIT_FAILURE);
	}

	if((format.fmt.pix.width != width) || (format.fmt.pix.height != height))
	{
		printf("Warning! Driver sending video at %dx%d!\n",
		       format.fmt.pix.width, format.fmt.pix.height);
		
	}
}

int init_cam(char *dev_name)
{
	fd = v4l2_open(dev_name, O_RDWR | O_NONBLOCK, 0);
	set_cam_settings(1280,720,V4L2_PIX_FMT_RGB24);
	return fd;
}

void start_stream(int fd)
{
	v_type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	
	xioctl(fd, VIDIOC_STREAMON, &v_type);
}

void close_cam(int fd, struct buffer *buffers)
{
	v_type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	
	xioctl(fd, VIDIOC_STREAMOFF, &v_type);
	
	/* Close mmap */
	for(i = 0; i < n_buffers; i++)
	{
		v4l2_munmap(buffers[i].start,buffers[i].length);
	}
	
	sleep(1);

	v4l2_close(fd);
}
