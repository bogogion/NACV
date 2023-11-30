#include <stdint.h>
#include <apriltag/apriltag.h>
#include "../camera/processing.h"
#include "../core/nacv.h"
/* Code for all configuration in NACV 
 	 Settings are viewed in JSON format using cJSON
	 Settings are then applied to a shared memory region so that the sub processors
	 can properly set themselves up without having to create super long function parameter lists whilst
	 also avoiding hardcode macros */

/* TODO: add ALL pi camera settings eventually (mainly the ISO related ones)*/

#ifndef CONFIG_GLOBAL_SHARE
#define CONFIG_GLOBAL_SHARE

#define MAX_CONFIG_FILE_LENGTH 512
typedef struct ctrl_share
{
	struct april_ctrls
	{
		int decision_threshold; /* Recommended 70                        */
		int max_tags;           /* Max tags that can be detected at once */
		float decimation;       /* Anything below 2.0 is set to 2.0      */
		int refine;             /* Bool for refining                     */
	} april;

	struct camera_ctrls
	{
		/* PiCamera specific controls */
		uint8_t auto_exposure;          /* True or false         */
		int     exposure_time_absolute; /* 0 - 10000             */
		int     brightness;             /* 0 - 100 		 */
		int     contrast;		
		int     saturation;
		int     red_balance;
		int     blue_balance;
		int     sharpness;
		int     rotate;
	} camera;

	struct meta_ctrls
	{
		int team_number;     /* Full team number                                */
		int camera_id;       /* ID of the Camera (user defined)                 */
		int port;            /* Port number, default 5805, only 5800-5810 valid */
		int controller_port; /* Port used by RoboRIO for controls, default 5800 */
		int processors;      /* Number of processors to use at runtime          */
	} meta;

} ctrl_share;

void json_to_ctrl_share(ctrl_share *ctrls, char *pathname);
void launch_config_memory(ctrl_share *ctrls); /* Puts ctrl structure into a shared memory region */
void cleanup_config_memory();
void load_camera_settings(ctrl_share ctrls);
#endif
