#include <linux/videodev2.h>
#include <linux/v4l2-common.h>
#include <linux/v4l2-controls.h>
#include <sys/mman.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <sys/time.h>
#define CLEAR(x) memset(&(x),0,sizeof(x))

struct buffer
{
	void   *start;
	size_t length;
}; 
/* Bugfer */
struct buffer *buffers;
static int fd = -1;
struct v4l2_buffer buf;
struct v4l2_requestbuffers req;
unsigned int n_buffers;

enum v4l2_buf_type type;

void init_mmap()
{
	CLEAR(req);
	type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	/* Init buffers */

	/* No check for capability of streaming.
	   This should already be known, and cameras using the uvcvideo drivers 
	   should already have the capabilities.

	   For future use, MAKE SURE that the camera has the capabilities, as the code
	   will run but not throw you any errors when you read from the device.
	*/

	req.count = 2;
	req.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	req.memory = V4L2_MEMORY_MMAP;
	
	if(-1 == ioctl(fd,VIDIOC_REQBUFS, &req))
	{
		printf("VIDIOC_REQBUFS");
		exit(EXIT_FAILURE);
	}

	if(req.count < 2)
	{
		printf("Insufficient buffer memory!");
		exit(EXIT_FAILURE);
	}

	buffers = calloc(req.count, sizeof(*buffers));
		
	if(!buffers)
	{
		printf("Out of memory!");
		exit(EXIT_FAILURE);
	}

	for(n_buffers = 0; n_buffers < req.count; n_buffers++)
	{
		CLEAR(buf);
	
		buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
		buf.memory = V4L2_MEMORY_MMAP;
		buf.index = n_buffers;
		
		if(-1 == ioctl(fd,VIDIOC_QUERYBUF, &buf))
		{
			printf("VIDEO_QUERYBUF");
			exit(EXIT_FAILURE);
		}

		buffers[n_buffers].length = buf.length;
		buffers[n_buffers].start = mmap(NULL, buf.length, PROT_READ | PROT_WRITE, MAP_SHARED, fd, buf.m.offset);

		if(MAP_FAILED == buffers[n_buffers].start)
		{
			printf("MAP_FAILED");
			exit(EXIT_FAILURE);
		}
	}

	for(int i = 0; i < n_buffers;i++)
	{
		CLEAR(buf);
		buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
		buf.memory = V4L2_MEMORY_MMAP;
		buf.index = i;
		if(-1 == ioctl(fd,VIDIOC_QBUF, &buf))
		{
			printf("VIDIOC_QBUF");
			exit(EXIT_FAILURE);
		}
	
	}
	
	type = V4L2_BUF_TYPE_VIDEO_CAPTURE;

	if(-1 == ioctl(fd,VIDIOC_STREAMON,&type))
	{
		printf("STREAMON FAILURE: %d %s \n",errno,strerror(errno));
		exit(EXIT_FAILURE);
	}
}

int main()
{
	struct v4l2_input input;
	struct v4l2_capability capability;
	int index;

	/* Init device, device name should be known already! */
	fd = open("/dev/video2", O_RDWR | O_NONBLOCK, 0);

	if(-1 == ioctl(fd, VIDIOC_G_INPUT, &index))
	{
		perror("VIDIOC_G_INPUT");
		exit(EXIT_FAILURE);
	}

	memset(&input, 0, sizeof(input));
	input.index = index;

	if(-1 == ioctl(fd,VIDIOC_ENUMINPUT, &input))
	{
		perror("VIDIOC_ENUMINPUT");
		exit(EXIT_FAILURE);
	}

	memset(&capability, 0 , sizeof(capability));

	if(-1 == ioctl(fd,VIDIOC_QUERYCAP,&capability))
	{
		perror("VIDIOC_QUERYCAP");
		exit(EXIT_FAILURE);
	}

	if(!(capability.capabilities & V4L2_CAP_VIDEO_CAPTURE))
	{
		perror("V4L2_CAP_VIDEO_CAPTURE: ");
		exit(EXIT_FAILURE);
	}


	printf("Current input: %s %u\n",input.name,input.type);
	printf("%s Capabilities: %u\n",input.name,capability.device_caps);
	printf("\n%s Info \nDriver: %s \nCard: %s \nBus_Info: %s\n",input.name, capability.driver, capability.card, capability.bus_info);
	
	struct v4l2_format fmt;
	fmt.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	fmt.fmt.pix.width       = 640;
	fmt.fmt.pix.height      = 480;
	fmt.fmt.pix.pixelformat = V4L2_PIX_FMT_JPEG;
	fmt.fmt.pix.field	= V4L2_FIELD_INTERLACED;


	/* Real men check for errors :) */
	if(-1 == ioctl(fd,VIDIOC_S_FMT,&fmt))
	{
		perror("FUCK YOU!!! (VIDIOC_S_FMT)");
		exit(EXIT_FAILURE);
	}

	init_mmap();
	FILE *fp;
	struct timeval tv;
	fd_set fds;
	char out_name[256];
	int r;
	for(int i = 0; i < 20; i++)
	{
		do
		{
			FD_ZERO(&fds);
			FD_SET(fd,&fds);

			tv.tv_sec = 2;
			tv.tv_usec = 0;
		
			r = select(fd+1,&fds,NULL,NULL,&tv);
		} while ((r == -1 && (errno = EINTR)));
		if(r == -1)
		{
			perror("select");
			return errno;
		}
		
		CLEAR(buf);
		buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
		buf.memory = V4L2_MEMORY_MMAP;
		ioctl(fd,VIDIOC_DQBUF, &buf);
		sprintf(out_name, "test%i.mpg",i);
		fp = fopen(out_name, "w");
	       	fprintf(fp,"0000");
		fwrite(buffers[buf.index].start,buf.bytesused, 1, fp);
		fclose(fp);	
		ioctl(fd,VIDIOC_DQBUF, &buf);
	}

	

	/* Close camera */
	ioctl(fd,VIDIOC_STREAMOFF,&type);
	close(fd);
	return 0;
}
