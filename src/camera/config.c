#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <apriltag/apriltag.h> /* For detector */
#include "config.h"
#include "camera.h"
void set_settings_from_config(char *pathname, apriltag_detector_t *td, struct calibration_data *cdata)
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
				else if(strcmp(temp_token,"Calibration") == 0)
				{
					cstate = CALIBRATION;
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
						case CALIBRATION:
							char *pend;
							
							if(strcmp("M_VALUE",temp_token) == 0)	
							{cdata->m = (float)strtof(temp_value,NULL);}
							else if(strcmp("B_VALUE",temp_token) == 0)
							{cdata->b = (float)strtof(temp_token,NULL);}
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
