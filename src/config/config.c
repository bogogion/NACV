#include "config.h"
#include "../../lib/cjson/cJSON.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
ctrl_share *config_memroy;
int config_cfd;

void json_to_ctrl_share(ctrl_share *ctrls, char *pathname)
{
	/* Get string value of file */
	FILE *fptr = fopen(pathname,"r");
	char buffer[MAX_CONFIG_FILE_LENGTH];
	memset(buffer,'\0',MAX_CONFIG_FILE_LENGTH);
	int size_of_file = fread(buffer,1,MAX_CONFIG_FILE_LENGTH,fptr);
	fclose(fptr);

	cJSON *file = cJSON_ParseWithLength(buffer,size_of_file);
	
	if(file == NULL)
	{
		printf("Warning! Error parsing JSON settings file. Check file for errors / existence!\n");
		cJSON_Delete(file);
		return;
	}


	/* WARNING Assumes all values are correct format. If not I have NO IDEA what will happen
	   Probably the start of the apocalypse or something!!! Remember kids, always check your types
	   (unless you're me) */

	// TODO: note to self, check these values with a memory viewer or something because these are never
	// deallocated? I assume it happens with cJSON_Delete but i really got no clue :D
	
	// TODO: another note to myself I am ASSUMING that valueint would just be 0 or 1 for boolean
	// have no idea, but i can't see any other alternative, just gonna assume lol
	/* AprilTag data */
	cJSON *apriltag = cJSON_GetObjectItemCaseSensitive(file, "april_settings");
	cJSON *dec_thres = cJSON_GetObjectItemCaseSensitive(apriltag,"decision_threshold");
	cJSON *max_tags = cJSON_GetObjectItemCaseSensitive(apriltag, "max_tags");
	cJSON *decimation = cJSON_GetObjectItemCaseSensitive(apriltag, "decimation");
	cJSON *refine = cJSON_GetObjectItemCaseSensitive(apriltag, "refine");
	
	ctrls->april.decision_threshold = dec_thres->valueint; 
	ctrls->april.max_tags = max_tags->valueint;
	ctrls->april.decimation = (float)decimation->valuedouble;
	ctrls->april.refine = refine->valueint;

	/* Camera data */
	cJSON *camera = cJSON_GetObjectItemCaseSensitive(file, "camera_settings");
	cJSON *auto_exposure = cJSON_GetObjectItemCaseSensitive(camera, "auto_exposure");
	cJSON *exposure_time_absolute = cJSON_GetObjectItemCaseSensitive(camera, "exposure_time");
	cJSON *brightness = cJSON_GetObjectItemCaseSensitive(camera, "brightness");
	cJSON *contrast = cJSON_GetObjectItemCaseSensitive(camera, "contrast");
	cJSON *saturation = cJSON_GetObjectItemCaseSensitive(camera, "saturation");
	cJSON *red_balance = cJSON_GetObjectItemCaseSensitive(camera, "red_balance");
	cJSON *blue_balance = cJSON_GetObjectItemCaseSensitive(camera, "blue_balance");
	cJSON *sharpness = cJSON_GetObjectItemCaseSensitive(camera, "sharpness");
	cJSON *rotate = cJSON_GetObjectItemCaseSensitive(camera, "rotate");

	ctrls->camera.auto_exposure = auto_exposure->valueint;
	ctrls->camera.exposure_time_absolute = exposure_time_absolute->valueint;
	ctrls->camera.brightness = brightness->valueint;
	ctrls->camera.contrast = contrast->valueint;
	ctrls->camera.saturation = saturation->valueint;
	ctrls->camera.red_balance = red_balance->valueint;
	ctrls->camera.blue_balance = blue_balance->valueint;
	ctrls->camera.sharpness = sharpness->valueint;
	ctrls->camera.rotate = rotate->valueint;

	/* Metadata */
	cJSON *meta = cJSON_GetObjectItemCaseSensitive(file, "meta");
	cJSON *team_number = cJSON_GetObjectItemCaseSensitive(meta, "team_number");
	cJSON *camera_id = cJSON_GetObjectItemCaseSensitive(meta, "camera_id");
	cJSON *port = cJSON_GetObjectItemCaseSensitive(meta, "port");
	cJSON *control_port = cJSON_GetObjectItemCaseSensitive(meta, "controller_port");
	cJSON *processors = cJSON_GetObjectItemCaseSensitive(meta, "processors");

	ctrls->meta.team_number = team_number->valueint;
	ctrls->meta.camera_id = camera_id->valueint;
	ctrls->meta.processors = processors->valueint;
	ctrls->meta.port = port->valueint;
	ctrls->meta.controller_port = control_port->valueint;

	cJSON_Delete(file);
	return;
}

void launch_config_memory(ctrl_share *ctrls) // TODO: find out why tf i thought of this?? (I KNOW NOW!!!)
{
	config_cfd = shm_open("nacv_config",O_CREAT | O_RDWR, S_IRWXU);
	ftruncate(config_cfd,sizeof(ctrl_share));
	config_memroy = mmap(NULL, sizeof(ctrl_share),PROT_READ | PROT_WRITE, MAP_SHARED, config_cfd, 0);
	memcpy(config_memroy,ctrls,sizeof(ctrl_share));
}

void cleanup_config_memory()
{
	shm_unlink("nacv_config");
	munmap(config_memroy,sizeof(ctrl_share));
}
