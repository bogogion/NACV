/* Defined structures and functions for multiprocessing of camera related functions. */
#include <stdint.h>

#ifndef SHARED_MULTIP
#define SHRRED_MULTIP

#define _M_READY_TO_PROCESS 1 << 7  /* 10000000 */   
#define _C_READY_TO_PROCESS 1       /* 00000001 */
#define _C_DATA_SET         1 << 4  /* 00010000 */
#define PROCESSING          1 << 5  /* 00100000 */
#define KILL                0xFF    /* 11111111 */

#define PROCESSORS          2       /* Number of processors needed at runtime */

/* TODO: figure out pose information */
typedef struct data
{
	struct metadata
	{
		uint8_t    identity;    /* Custom ID given to process           */
		int32_t    pid;         /* PID of process                        */
		uint8_t    tags_found;  /* Amount of tags found in detection     */
	} meta;

	struct aprildata
	{
		float      dist[10];    /* Distance from tag                     */
		uint32_t   area[10];    /* Area in pixels of tag                 */
		uint8_t    id[10];      /* ID of the tag                         */
		float      angle[10];   /* Angle from the camera (WIP)           */
	} aprild;
} full_process_data;

struct data_share
{
	unsigned char processes[PROCESSORS];
	full_process_data  data[PROCESSORS];
};

#endif
