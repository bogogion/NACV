#include <apriltag/common/zarray.h>
#include <apriltag/apriltag.h>
#include <apriltag/tag16h5.h>
#include <apriltag/common/image_u8.h>
#include "../camera/processing.h"
#include "../camera/camera.h"
#include "../server/server_client.h"
#include "shared.h"
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <sys/time.h>
#include <pthread.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <fcntl.h>

/* Settings */
#define LOWEST_TAG  1
#define HIGHEST_TAG 8

int run = 1;

/* Handles Ctrl + C end of loop */
void intHandler(int useless)
{
	run = 0;	
}

int main(int argc, char *argv[])
{
	struct calibration_data cdata;
	
	int sfd = shm_open("nacv_ctrl",O_CREAT | O_RDWR, S_IRWXU);	
	ftruncate(sfd, sizeof(struct data_share));

	/* Default camera device is video0 */
	int fd = init_everything(CAMERA_WIDTH,CAMERA_HEIGHT);

	/* Check for our magic control */
	if(check_for_sensormode())
	{
		set_camera_settings(ID_CLASS_USER,0x009819e0,7);
	}

	struct data_share *datashare;
	datashare = mmap(NULL, sizeof(struct data_share), PROT_READ | PROT_WRITE, MAP_SHARED, sfd, 0);

	/* Start server */
	pthread_t thread;	int iret1;
	iret1 = pthread_create(&thread,NULL,thread_function,NULL);

	start_stream(fd);

	/* Launch our seperate processors after stream is started */
	launch_processes();

	/* Handles Ctrl + C end of loop */
	signal(SIGINT, intHandler);

	while(run)
	{
		mainloop_shm();
	}

	/* Cleanup */
	shm_unlink("nacv_ctrl");	

	pthread_cancel(thread);

	/* Send kill signal to child processors */
	for(int i = 0; i < PROCESSORS; i++)
	{
		kill(datashare->data[i].meta.pid, SIGTERM);		
	}

	munmap(datashare, sizeof(struct data_share));
	close_cam(fd);
	return 0;
}
