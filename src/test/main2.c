#include <unistd.h>
#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include "../core/shared.h"


int main(int argc, char *argv[])
{
	int fd, id;
	char shm_name[255];
	struct data_share *data;

	/* Options */
	if(argc == 3)
	{
		id = atoi(argv[1]);
		if(strlen(argv[2]) < 255)
		{
			memcpy(shm_name, argv[2], strlen(argv[2]));
		}
	}

	fd = shm_open(shm_name, O_RDWR, 0);
	
	/* Open our shm */
	data = mmap(NULL, sizeof(struct data_share), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);

	printf("0x%i\n",data);

	/* Set our ID */
	data->data[0].meta.identity = id;
	data->data[0].meta.pid = getpid();

	/* Set our start flag */
	data->processes.individual[0] = _C_READY_TO_PROCESS;
	while(1)
	{
		switch(data->processes.individual[0])	
		{
			case _M_READY_TO_PROCESS:
				/* Start processing */
				data->processes.individual[0] = PROCESSING;
				/* Processing code */
				srand(time(NULL));	
				data->data[0].aprild.dist  = rand() % 60;
				data->data[0].aprild.area  = rand() % 150000;
				data->data[0].aprild.id    = rand() % 8;
				data->data[0].aprild.angle = rand() % 180;
				
				sleep(3); 

				data->processes.individual[0] = _C_DATA_SET;
				break;
		}
	}

	munmap(data,sizeof(struct data_share));
	shm_unlink(shm_name);

	return 0;
}
