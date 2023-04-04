#include <apriltag/apriltag.h>
#include "../math/d_math.h"

enum class_state{USER,CAMERA,DETECTOR,CALIBRATION};

void set_settings_from_config(char *pathname, apriltag_detector_t *td, struct calibration_data *cdata);
int set_apriltag_setting(apriltag_detector_t *td, char *setting_name, char *value);
