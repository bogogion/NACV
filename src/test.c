#include "config/config.h"
#include <stdio.h>


int main()
{
	ctrl_share test;
	json_to_ctrl_share(&test,"nacv.json");
	
	printf("r-%i maxt-%i dect-%i dec-%f\n",test.april.refine, test.april.max_tags, test.april.decision_threshold, test.april.decimation);
	printf("ae-%i et-%i b-%i c-%i\n",test.camera.auto_exposure, test.camera.exposure_time_absolute, test.camera.brightness, test.camera.contrast);
	printf("s-%i rb-%i bb-%i s-%i ro-%i\n",test.camera.saturation,test.camera.red_balance,test.camera.blue_balance,test.camera.sharpness,test.camera.rotate);
	printf("tn-%i ci-%i pc-%i p-%i cp-%i\n",test.meta.team_number,test.meta.camera_id,test.meta.processors,test.meta.port,test.meta.controller_port);
	return 0;
}	
