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
#include "../../lib/libv4l/include/libv4l2.h"
#include "camera.h"
#include "processing.h"
#include "../core/shared.h"
#include "../core/nacv.h"

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
struct v4l2_control         user_ctrl;
struct v4l2_ext_control     single_ctrl;
struct v4l2_ext_controls    cam_ctrl;
struct v4l2_format          format;
struct v4l2_buffer          v_buf;
struct v4l2_requestbuffers  v_request;
struct v4l2_queryctrl       v_ctrlq;
enum v4l2_buf_type          v_type;
fd_set                      fds;
struct timeval              tv;
int                         r, fd = -1, shfd, shfd_ctrl, increment;
struct data_share           *datas;
unsigned int                i, n_buffers;
static struct buffer        *buffers;

int check_for_sensormode()
{
	v_ctrlq.id = 0x009819e0; /* sensor mode */

	xioctl(fd, VIDIOC_QUERYCTRL, &v_ctrlq);

	if(v_ctrlq.flags == V4L2_CTRL_FLAG_DISABLED)
	{
		printf("Warning! No V4L2 PiCamera Sensor Mode control exposed! Please use modified kernel for best performance. Driver forced at 1920x1080 mode.\n");
		return 0;
	}
	return 1;
}

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

int init_everything(int width, int height)
{
	int fd = init_cam("/dev/video0",width,height);
	init_shm(width * height * 3);
	return fd;
}

void init_shm(unsigned int buffer_size)
{

	CLEAR(v_request);

	/* Doing several buffers with userptr causes a crash with V4L2 */
	v_request.count = 1;
	v_request.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	v_request.memory = V4L2_MEMORY_USERPTR;

	/* Check if the jawn work */
	if(-1 == ioctl(fd, VIDIOC_REQBUFS, &v_request))
	{
		if(EINVAL == errno)
		{
			printf("DEVICE DOES NOT SUPPORT USERPOINTER, EXITING!\n");
		} else {
			return;
		}
	}

	buffers = calloc(1, sizeof(*buffers));
	
	/* Intiliaze pointers */
	buffers[0].length = buffer_size;

	/* Create our shm */
	shfd = shm_open("nacv_data", O_CREAT | O_RDWR, S_IRWXU);
	ftruncate(shfd, buffer_size);

	buffers[0].start = mmap(NULL, buffer_size, PROT_READ | PROT_WRITE, MAP_SHARED, shfd, 0);

	/* Queue buffers */
	CLEAR(v_buf);
	v_buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	v_buf.memory = V4L2_MEMORY_USERPTR;
	v_buf.index = 0;
	v_buf.m.userptr = (unsigned long)buffers[0].start;
	v_buf.length = buffer_size;

	xioctl(fd,VIDIOC_QBUF,&v_buf);

	shfd_ctrl = shm_open("nacv_ctrl", O_RDWR, 0);
	datas = mmap(NULL, sizeof(struct data_share), PROT_READ | PROT_WRITE, MAP_SHARED, shfd_ctrl, 0);

}

void cleanup_shm(unsigned int buffer_size)
{
	munmap(buffers[0].start,buffer_size);
	shm_unlink("nacv_data");
}

/* Mainloop for shared memory system. Handles all calls accordingly. */

void mainloop_shm()
{	
	do {
		FD_ZERO(&fds);
		FD_SET(fd, &fds);
		 
		/* Timeout. */
		tv.tv_sec = 2;
		tv.tv_usec = 0;

		r = select(fd + 1, &fds, NULL, NULL, &tv);
	} 
	while ((r == -1 && (errno = EINTR)));
        
	if (r == -1) {
		perror("select");
		return;
	}

	xioctl(fd, VIDIOC_DQBUF, &v_buf);

	/* We requeue and return to avoid any of the processes from doing the same frame multiple times */

	for(int i = 0; i < PROCESSORS; i++)
	{
		switch(datas->processes[i])
		{
			case _C_READY_TO_PROCESS:
				/* Called once a process is setup or if it doesn't detect anything */
				datas->processes[i] = _M_READY_TO_PROCESS;
				printf("Nothing found, frame %i\n",increment++);
				xioctl(fd, VIDIOC_QBUF, &v_buf);
				return;
			case _C_DATA_SET:
				/* Handle data */
				
				printf("Data was set, frame %i\n",increment++);
				
				/* Loop through our tags and send them jawns over! */	
				for(int j = 0; j < datas->data[i].meta.tags_found; j++)
				{
					send_message(&datas->data[i].aprild[j]);
				}
				/* Once done give ok to process */
				datas->processes[i] = _M_READY_TO_PROCESS;
				xioctl(fd, VIDIOC_QBUF, &v_buf);
				return;
			case PROCESSING:
				break; /* Ignore */
		}
	}

	/* Requeue buffers */
	xioctl(fd, VIDIOC_QBUF, &v_buf);
}

void apriltag_debug(struct apriltag_stack astack)
{	
	do {
		FD_ZERO(&fds);
		FD_SET(fd, &fds);
		 
		/* Timeout. */
		tv.tv_sec = 2;
		tv.tv_usec = 0;

		r = select(fd + 1, &fds, NULL, NULL, &tv);
	} 
	while ((r == -1 && (errno = EINTR)));
        
	if (r == -1) {
		perror("select");
		return;
	}
	xioctl(fd, VIDIOC_DQBUF, &v_buf);

	/* Get detection */
	convert_rgb24_proper(CAMERA_WIDTH,CAMERA_HEIGHT,astack.im->stride,(uint8_t*)buffers[0].start,astack.im);
	zarray_t *detections = apriltag_detector_detect(astack.td,astack.im,astack.decim,DECISION_THRESHOLD,MAX_TAGS);

	zarray_destroy(detections);

	xioctl(fd, VIDIOC_QBUF, &v_buf);
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

	cleanup_shm(CAMERA_WIDTH * CAMERA_HEIGHT * 3);
	
	sleep(1);

	v4l2_close(fd);
}
