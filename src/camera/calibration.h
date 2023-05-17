#include "../math/d_math.h"

#ifndef CALIBRATION_H
#define CALIBRATION_H

/* returns true or false based on if it already exists, if false it generates it */
unsigned char check_for_calibration_file(const char* filepath);
void set_calibration_from_file(struct calibration_data *cdata, const char* filepath);

#endif
