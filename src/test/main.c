#include <unistd.h>
#include <stdio.h>
#include <pthread.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <signal.h>
#include "../core/shared.h"

int run = 1;
void intHandler(int useless)
{ run = 0; }

int main()
{
	int fd;
	struct data_share *data;

	/* Open our shm as owner with full permissions */
	fd = shm_open("test_shm", O_CREAT | O_RDWR, S_IRWXU);
	
	/* Set size of data share */
	ftruncate(fd, sizeof(struct data_share));

	/* Open our shm */
	data = mmap(NULL, sizeof(struct data_share), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);

	printf("0x%i\n",data);

	signal(SIGINT, intHandler);
	/* keep everything open until closed */
	while(run)
	{
		switch(data->processes.individual[0])
		{
			case _C_READY_TO_PROCESS:
				/* To test set mode to ready to process */
				data->processes.individual[0] = _M_READY_TO_PROCESS;
				break;
			case _C_DATA_SET:
				/* Print metadata (excluding frame)*/
				printf("ID %i PID %i\n",data->data[0].meta.identity, data->data[0].meta.pid);
				
				/* Print AprilTag data */
				printf("DIST %f\nAREA %i\n",data->data[0].aprild.dist, data->data[0].aprild.area);
				printf("ID %i\nANGLE %f\n",data->data[0].aprild.id, data->data[0].aprild.angle);

				/* Resume ability to process */
				data->processes.individual[0] = _M_READY_TO_PROCESS;
				break;
			case PROCESSING:
				printf("PROCESSING...\n");
				break;
		}
		sleep(1);
	}
	
	munmap(data,sizeof(struct data_share));
	shm_unlink("test_shm");

	return 0;
}
