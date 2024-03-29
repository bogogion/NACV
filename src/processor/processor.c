/* 
   NACV Co-Processor
   This processor handles the normal AprilTag pipeline
   Queued with userpointer (data pointer) by the main process
   These processes should NEVER be called manually unless you know
   what you're doing! 
*/ 

#include "../core/nacv.h"

#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <apriltag/apriltag.h>
#include <apriltag/common/zarray.h>
#include <apriltag/tag16h5.h>

uint8_t               run = 1;
int                   identity; /* Identity of process */
int                   sfd, cfd, configfd;
int                   size = SIZE;
int                   max_tags, dec_thres;
char                  shm_path[255];
char                  ctrl_path[255];
char                  *data;
struct data_share     *ds;
unsigned char         *control_byte;
struct apriltag_stack april_stack;
ctrl_share            *config;
/* Handles signal */
void sigHandler(int useless)
{ run = 0; }

void mainloop()
{
	*control_byte = _C_READY_TO_PROCESS;
	int tags, i;

	while(run)
	{
		switch(*control_byte)
		{
			case _M_READY_TO_PROCESS:
				*control_byte = PROCESSING;

				/* Setup our detection */
				convert_rgb24_proper(CAMERA_WIDTH, CAMERA_HEIGHT, april_stack.im->stride, data, april_stack.im);
				zarray_t *detections = apriltag_detector_detect(april_stack.td, april_stack.im, april_stack.decim, dec_thres, max_tags);			
				
				tags = zarray_size(detections);
				ds->data[identity].meta.tags_found = tags;

				if(tags != 0)
				{
					for(i = 0;i < tags; i++)
					{
						zarray_get(detections, i, &april_stack.det);

						/* TODO: finish processing */
						ds->data[identity].aprild[i].id = april_stack.det->id;
						ds->data[identity].aprild[i].area = grab_area(april_stack.det->p);
						ds->data[identity].aprild[i].center_x = april_stack.det->c[0];
						ds->data[identity].aprild[i].center_y = april_stack.det->c[1];	
					}
				} else
				{
					/* If nothing found, return out to save processing */
					zarray_destroy(detections);
					*control_byte = _C_READY_TO_PROCESS;
					break;
				}

				zarray_destroy(detections);

				/* Only set if we actually set something */
				*control_byte = _C_DATA_SET;
				break;
		}
	}
}

int main(int argc, char *argv[])
{
	if(argc != 5)
	{
		printf("Too few arguments produced. Ex. nacv-processor {ID} {shm_name} {shm_size} {ctrl_name} \n");
		return 1;
	}
	
	/* Fill in our information */
	identity = atoi(argv[1]);
	memcpy(shm_path, argv[2], strlen(argv[2]));
	size = atoi(argv[3]);
	memcpy(ctrl_path, argv[4], strlen(argv[4]));

	/* Setup access to our memory */
	sfd  = shm_open(shm_path, O_RDONLY, 0);
	data = mmap(NULL, size, PROT_READ, MAP_SHARED, sfd, 0);

	/* Setup access to the data share / controller */
	cfd  = shm_open(ctrl_path, O_RDWR, 0);
	ds   = mmap(NULL, sizeof(struct data_share), PROT_READ | PROT_WRITE, MAP_SHARED, cfd, 0);

	/* Setup access to the configuration share */
	configfd = shm_open("nacv_config",O_RDONLY,0);
	config   = mmap(NULL, sizeof(ctrl_share), PROT_READ, MAP_SHARED, configfd, 0);

	/* Setup our control_byte for quick access */
	control_byte = &ds->processes[identity];

	/* Setup identifying information */
	ds->data[identity].meta.pid      = getpid();
	ds->data[identity].meta.identity = identity;

	/* Setup AprilTag */
	create_apriltag_stack(&april_stack, CAMERA_WIDTH, CAMERA_HEIGHT);	
	
	/* Apply our detector settings */
	max_tags = config->april.max_tags;
	april_stack.td->quad_decimate = config->april.decimation;
	april_stack.td->refine_edges = config->april.refine; 
	dec_thres = config->april.decision_threshold;
	
	
	for(int i = 0; i < max_tags; i++)
	{
		ds->data[identity].aprild[i].camera_id = config->meta.camera_id; 
	}

	/* Run mainloop and signal handling */
	signal(SIGTERM, sigHandler);
	mainloop();

	/* Cleanup */
	clean_apriltag_stack(&april_stack);
	munmap(data,size);
	shm_unlink(shm_path);
	munmap(ds,sizeof(struct data_share));
	shm_unlink(ctrl_path);
	munmap(config,sizeof(ctrl_share));
	shm_unlink("nacv_config");

	return 0;
}
