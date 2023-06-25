#include <apriltag/apriltag.h>
#include "../../lib/cjson/cJSON.h"

#ifndef APRIL_CONFIG
#define APRIL_CONFIg

#define MAX_APRIL_JSON_SIZE 1024

void set_april_settings_from_config_file(char *pathname, apriltag_detector_t *td);
void save_april_settings_to_disk(char *savepath, apriltag_detector_t td);
void set_settings_for_detector(apriltag_detector_t *td,
															 int nthreads, float quad_decimate,
															 float quad_sigma, uint8_t refine,
															 double sharpening);
void set_settings_for_qtp(struct apriltag_quad_thresh_params *qtp,
													int min_pixels, int max_nmaxima, float crit_rad,
													float cos_crit_rad, float max_line_fit,
													int min_white_black_diff, int deglitch);

#endif
