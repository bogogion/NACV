#include <apriltag/apriltag.h>
#include "../math/d_math.h"

#ifndef CONFIG_H
#define CONFIG_H

enum class_state{USER,CAMERA,DETECTOR};

void set_settings_from_config(char *pathname, apriltag_detector_t *td);
int set_apriltag_setting(apriltag_detector_t *td, char *setting_name, char *value);
void set_cdata_from_file(const char *pathname, struct calibration_data *cdat);
void ssfp_helper(uint32_t user[][2], uint32_t cam[][2], int user_size, int camera_size);
void set_settings_from_profile(uint8_t profile);

/* Below are predefined settings based on each supported sensor.
   Currently supported sensors:

   [1] Pi Camera 1-1.3 (OmniVision-OV5647) 
   [2] Microsoft LifeCam HD-3000 (usb webcam)

   Naming Schema:
   C1 (Camera 1)
   1U (Profile 1 User Settings)
   1C (Profile 1 Camera Settings) 			      

   Some settings are ommitted due to not being useful.
   ALL definitions for settings are in config.c
*/


/* ---- PI Camera v1-1.3 ---- */
/*        Profile One         */
/*         Defaults           */

#define PROFILE_C1_1U_SIZE 9
#define PROFILE_C1_1C_SIZE 9
#define PROFILE_C1_1_ID    0x1

/* ---- Microsoft Webcam ---- */
/*        Profile One         */
/*            Test            */
#define PROFILE_C2_1U_SIZE 4
#define PROFILE_C2_1C_SIZE 1
#define PROFILE_C2_1_ID    0x2


#endif
