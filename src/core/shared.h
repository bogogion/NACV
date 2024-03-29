/* Defined structures for multiprocessing of camera related functions. */
#include <stdint.h>

#ifndef SHARED_MULTIP
#define SHARED_MULTIP

#define _M_READY_TO_PROCESS 1 << 7  /* 10000000 */   
#define _C_READY_TO_PROCESS 1       /* 00000001 */
#define _C_DATA_SET         1 << 4  /* 00010000 */
#define PROCESSING          1 << 5  /* 00100000 */

#define PROCESSORS          4       /* Number of processors needed at runtime     */
#define MAX_TAGS            10      /* Needs to also be set in nacv.json TODO: do some dynamic memory garbage */

/* TODO: figure out pose information */
typedef struct data
{
	struct metadata
	{
		uint8_t    identity;    /* Custom ID given to process        */
		int32_t    pid;         /* PID of process                    */
		uint8_t    tags_found;  /* Amount of tags found in detection */
	} meta;

	/* This struct is used for sending data. Make sure it aligns to multiples of 4 bytes */
	/* This struct is passed directly into the server to be passed to reduce latency     */
	/* allowing for better data transfer. That's why camera id is included               */
	
	struct aprildata
	{
		uint32_t   camera_id;   /* ID of Camera                      */
		float      dist;        /* Distance from tag                 */
		uint32_t   area;        /* Area in pixels of tag             */
		uint32_t   id;          /* ID of the tag                     */
		float      angle;       /* Angle from the camera (WIP)       */
		double     center_x;    /* Center X tag in image             */
		double     center_y;    /* Center Y tag in image             */
	} aprild[MAX_TAGS];
} full_process_data;

struct data_share
{
	int32_t main_pid;	/* PID of main NACV process */
	unsigned char processes[PROCESSORS];
	full_process_data  data[PROCESSORS];
};

#endif
