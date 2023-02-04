#include <apriltag/apriltag.h>

void set_settings_from_config(char *pathname, apriltag_detector_t *td);
int set_apriltag_setting(apriltag_detector_t *td, char *setting_name, char *value);
