#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <apriltag/apriltag.h> /* For detector */
#include "config.h"
#include "camera.h"

/* I understand that this is a mess, I have no clue how to do it any other way. TODO: make this better! */
void set_settings_from_config(char *pathname, apriltag_detector_t *td)
{
	char buf[255];
	char temp_token[255];
	char temp_value[255];
	
	unsigned int class_mode; /* 1 for class read, 0 for non class read */
	unsigned int value_mode; /* 1 for value read, 0 for non value read */
	unsigned int t_pos = 0;
	unsigned int v_pos = 0;

	enum class_state cstate;

	FILE *fp;
	fp = fopen(pathname,"r");

	while(fgets(buf,255,fp) != NULL)
	{
		/* Loop through each line */
		for(int i = 0; i < strlen(buf); i++)
		{
			if(buf[i] == ' ')
			{
				continue;
				/* Ignore whitespace */
			}

			/* # for comments */

			if(buf[i] == '#')
			{
				break;
			}

			/* Class relization */
			if(buf[i] == '[')
			{
				class_mode = 1;
				continue;
			}

			if(class_mode && buf[i] != ']')
			{
				temp_token[t_pos] = buf[i];
				t_pos++;
				continue;
			}
		       	else
			{
				class_mode = 0;
				if(strcmp(temp_token,"Camera") == 0)
				{
					cstate = CAMERA;
					CLEAR(temp_token);
					t_pos = 0;
					break;
				}
				else if(strcmp(temp_token,"User") == 0)
				{
					cstate = USER;
					CLEAR(temp_token);
					t_pos = 0;
					break;
				}
				else if(strcmp(temp_token,"Detector") == 0)
				{
					cstate = DETECTOR;
					CLEAR(temp_token);
					t_pos = 0;
					break;
				}
			}

			/* Read values and act on them! */
			if(class_mode == 0)
			{
				/* Finish of statement, set setting accordingly */
				if(buf[i] == ';')
				{
					/* Act based on which class is active */
					switch(cstate)
					{
						int id;
						int value;
						/* In reality, you handle user and camera the same way, just with a different class code */
						case USER:
							id = (int)strtol(temp_token,NULL,16);
							value = atoi(temp_value);
							
							printf("%X %i\n",id,value);
							
							set_camera_settings(ID_CLASS_USER,id,value);
							CLEAR(temp_token);
							CLEAR(temp_value);

							break;
						case CAMERA:
							id = (int)strtol(temp_token,NULL,16);
							value = atoi(temp_value);
							set_camera_settings(ID_CLASS_CAMERA,id,value);

							CLEAR(temp_token);
							CLEAR(temp_value);

							break;
						case DETECTOR:
							if(set_apriltag_setting(td,temp_token,temp_value) > 0)
							{
								printf("AprilTag detector setting: %s set!\n",temp_token);
							}
						       	else
							{
								printf("AprilTag detector setting: %s not found! :(\n",temp_token);	
							}

							CLEAR(temp_token);
							CLEAR(temp_value);

							break;
					}
					CLEAR(temp_token);
					CLEAR(temp_value);
					t_pos = 0;
					v_pos = 0;
					value_mode = 0;
					continue;
				}

				if(value_mode)
				{

					temp_value[v_pos] = buf[i];
					v_pos++;
					continue;
				}
				else if(buf[i] != ':')
				{
					temp_token[t_pos] = buf[i];
					t_pos++;
					continue;
				}
				else
				{
					value_mode = 1;
					continue;
				}
			}
		}
		value_mode = 0;
		v_pos = 0;
		t_pos = 0;
	}
	fclose(fp);
}

int set_apriltag_setting(apriltag_detector_t *td, char *setting_name, char *value)
{
	/* Set setting */
	if(strcmp(setting_name,"N_THREADS") == 0)
	{
		td->nthreads = atoi(value);
		return 1;
	} 
	else if(strcmp(setting_name,"QUAD_DECIMATE") == 0)
	{
		td->quad_decimate = (float)atof(value);
		return 1;
	}
	else if(strcmp(setting_name,"QUAD_SIGMA") == 0)
	{
		td->quad_sigma = (float)atof(value);
		return 1;
	}
	else if(strcmp(setting_name,"REFINE_EDGES") == 0)
	{
		td->refine_edges = atoi(value);
		return 1;
	}
	else if(strcmp(setting_name,"DECODE_SHARP") == 0)
	{
		td->decode_sharpening = (float)atof(value);
		return 1;
	}
	else
	{
		/* If none of these found return -1 */
		return -1;
	}


}

/* Definitions of Camera 1 Profile 1 (Defaults) */

/* User Controls */
uint32_t c1_1user_settings[9][2] =
{
        {0x980900, 50},      // Brightness                0-100
        {0x980901, 0},       // Contrast                  -100-100
 	{0x980902, 0},       // Saturation                -100-100
        {0x98090e, 1000},    // Red Balance               1-7999
        {0x98090f, 1000},    // Blue Balance              1-7999
        {0x98091b, 0},       // Sharpness                 -100-100
 	{0x98091f, 0},       // Color Effects             0-15 (menu)
 	{0x980922, 0},       // Rotation                   0-360 (90 step)
	{0x98092a, 32896}    // CBCR Color Effects        0-65535
};

/* Camera Controls */
uint32_t c1_1camera_settings[9][2] =
{
        {0x9a0901, 0},       // Auto Exposure             0 (Auto) 1 (Manual)
        {0x9a0902, 1000},    // Exposure Time             1-10000
        {0x9a0913, 12},      // Auto Exposure Bias        0-24 (menu)
        {0x9a0914, 1},       // White Balance Auto Preset 0-10 (menu)
        {0x9a0916, 0},       // Image Stabilization       0-1 (bool)
        {0x9a0917, 0},       // Iso Sensitivity           0-4 (menu)
        {0x9a0918, 1},       // Iso Sensitivity Auto      0 (Manual) 1 (Auto)
        {0x9a0919, 0},       // Exposure Metering Mode    0-3 (menu)
        {0x9a091a, 0}        // Scene Mode                0, 8, 11 (menu)
};

uint32_t c2_1user_settings[4][2] =
{
	{0x980900,133},
	{0x980901,5},
	{0x980902,83},
	{0x98091b,40}
};

uint32_t c2_1camera_settings[1][2] =
{
	{0x9a0901,3}
};

void ssfp_helper(uint32_t user[][2], uint32_t cam[][2], int user_size, int camera_size)
{
	int i;
	for(i = 0; i < user_size; i++)
	{
		set_camera_settings(ID_CLASS_USER, user[i][0], user[i][1]);
	}
	
	for(i = 0; i < camera_size; i++)
	{
		set_camera_settings(ID_CLASS_CAMERA, cam[i][0], cam[i][1]);
	}
}

void set_settings_from_profile(uint8_t profile)
{
	switch(profile)
	{
		case PROFILE_C1_1_ID:
			/* Camera 1 Profile 1 */
			ssfp_helper(c1_1user_settings, c1_1camera_settings, PROFILE_C1_1U_SIZE, PROFILE_C1_1C_SIZE);
			break;
		case PROFILE_C2_1_ID:
			ssfp_helper(c2_1user_settings, c2_1camera_settings, PROFILE_C2_1U_SIZE, PROFILE_C2_1C_SIZE);
			break;
		default:
			printf("No profile selected for camera. Defaults used.\n");
	}
}
