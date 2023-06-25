#include "april.h"
#include <stdio.h>
#include <stdlib.h>

void set_april_settings_from_config_file(char *pathname, apriltag_detector_t *td)
{
	/* Parse the JSON */
	FILE *fp = fopen(pathname, "r");
	char buffer[MAX_APRIL_JSON_SIZE];

	fread(buffer, 1, MAX_APRIL_JSON_SIZE, fp);
	fclose(fp);

	cJSON *json = cJSON_Parse(buffer);

	int nthreads;
	float quad_s, quad_d;
	uint8_t refine;
	double sharpening;

	cJSON *threads = cJSON_GetObjectItemCaseSensitive(json,"nthreads");
	if(cJSON_IsNumber(threads))
	{
		td->nthreads = threads->
	}



	cJSON_Delete(json);
}
