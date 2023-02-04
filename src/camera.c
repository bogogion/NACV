#include <stdio.h>
#include <malloc.h>
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
/* Tag detection*/
#include <apriltag/apriltag.h>
#include <apriltag/tag16h5.h>
#include <apriltag/common/zarray.h>
/* Neccessary camera code */
#include "../lib/libv4l/include/libv4l2.h"
#include "camera.h"
#include "processing.h"

void xioctl(int fh, int request, void *arg)
{
	int r;
	do {
		r = v4l2_ioctl(fh,request,arg);
	} while (r == -1 && ((errno == EINTR) || (errno == EAGAIN)));

	if(r == -1)
	{
		perror("XIOCTL_CALL");
		exit(EXIT_FAILURE);
	}
}

/* Declare variables */
struct v4l2_ext_control     single_ctrl;
struct v4l2_ext_controls    cam_ctrl;
struct v4l2_format	    format;
struct v4l2_buffer	    v_buf;
struct v4l2_requestbuffers  v_request;
enum v4l2_buf_type	    v_type;
fd_set			    fds;
struct timeval		    tv;
int			    r, fd = -1;
unsigned int		    i, n_buffers;
static struct buffer        *buffers;

void set_camera_settings(uint32_t ctrl_class, uint32_t id, int32_t value)
{
	/* Ctrl handler */
	cam_ctrl.ctrl_class = ctrl_class;
	cam_ctrl.count = 1;

	/* Setting to be changed */
	single_ctrl.id = id;
	single_ctrl.value = value;

	cam_ctrl.controls = &single_ctrl;

	xioctl(fd, VIDIOC_S_EXT_CTRLS, &cam_ctrl);
}

int init_everything(int width, int height, char *dev_name)
{
	int fd = init_cam("/dev/video0",width,height);
	init_mmap();
	return fd;
}

/* Create a 2 buffer memory map of the device */
void init_mmap()
{
	CLEAR(v_request);
	v_request.count = 4;
	v_request.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	v_request.memory = V4L2_MEMORY_MMAP;
	xioctl(fd, VIDIOC_REQBUFS, &v_request);
	
	buffers = calloc(v_request.count, sizeof(*buffers));
	
	/* Initialize buffers and assign them */
	for(n_buffers = 0; n_buffers < v_request.count; n_buffers++)
	{
		CLEAR(v_buf);

		v_buf.type   = V4L2_BUF_TYPE_VIDEO_CAPTURE;
		v_buf.memory = V4L2_MEMORY_MMAP;
		v_buf.index  = n_buffers;
		
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
		xioctl(fd, VIDIOC_QBUF, &v_buf);
	}
}

zarray_t* get_detections(apriltag_detector_t *td, image_u8_t *im)
{	
	do {
               	FD_ZERO(&fds);
               	FD_SET(fd, &fds);
		 
                /* Timeout. */
             	tv.tv_sec = 2;
		tv.tv_usec = 0;

               	r = select(fd + 1, &fds, NULL, NULL, &tv);
        } while ((r == -1 && (errno = EINTR)));
        if (r == -1) {
               perror("select");
	       return errno;
        }

        /* CLEAR(v_buf);
        v_buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
        v_buf.memory = V4L2_MEMORY_MMAP; */
        xioctl(fd, VIDIOC_DQBUF, &v_buf);
	convert_rgb24_proper(CAMERA_WIDTH,CAMERA_HEIGHT,im->stride,(uint8_t*)buffers[v_buf.index].start,im);

	zarray_t *detections = apriltag_detector_detect(td,im);
	/* Requeue buffers */
	xioctl(fd, VIDIOC_QBUF, &v_buf);
	return detections;
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

int init_cam(char *dev_name, int width, int height)
{
	fd = v4l2_open(dev_name, O_RDWR | O_NONBLOCK, 0);
	set_cam_settings(width,height,V4L2_PIX_FMT_RGB24);
	return fd;
}

void start_stream(int fd)
{
	v_type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	
	xioctl(fd, VIDIOC_STREAMON, &v_type);
}

void close_cam(int fd)
{
	v_type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	xioctl(fd, VIDIOC_STREAMOFF, &v_type);

	/* User warning*/
	printf("Closing camera, goodbye cruel world!\n");

	/* Close mmap */
	for(i = 0; i < n_buffers; i++)
	{
		v4l2_munmap(buffers[i].start,buffers[i].length);
	}
	
	sleep(1);

	v4l2_close(fd);
}
