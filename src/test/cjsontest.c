#include "../../lib/cjson/cJSON.h"
#include <stdio.h>
#include <stdlib.h>

int main()
{
	FILE *fp = fopen("test.json","r+");

	char buf[256];
	int len = fread(buf, 1, sizeof(buf),fp);
	fclose(fp);

	cJSON *json = cJSON_Parse(buf);

	cJSON *test = cJSON_GetObjectItemCaseSensitive(json, "test");
	cJSON *test2 = cJSON_GetObjectItemCaseSensitive(json, "test2");
	printf("%s\n",test->valuestring);

	if(cJSON_IsNumber(test2))
	{
		printf("%i\n",test2->valueint);
	}

	cJSON_Delete(json);
	return 0;
}
