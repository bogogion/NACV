#include <apriltag/apriltag.h>
#include "../math/d_math.h"

#ifndef CONFIG_H
#define CONFIG_H

enum class_state{USER,CAMERA,DETECTOR};

void set_settings_from_config(char *pathname, apriltag_detector_t *td);
int set_apriltag_setting(apriltag_detector_t *td, char *setting_name, char *value);
#endif
