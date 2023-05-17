#include <apriltag/common/image_u8.h>
#include <apriltag/common/math_util.h>
#include <stdio.h>
#include <stdlib.h>

#ifndef PROCESSING_H
#define PROCESSING_H

#define DEFAULT_ALIGNMENT_U8 96

int generate_stride(int width, unsigned int alignment);
void convert_rgb24_proper(int width, int height, int stride, uint8_t *inbuf, image_u8_t *im);
image_u8_t* create_image_u8(int width, int height, int stride);
void destroy_image_u8(image_u8_t *im);
#endif
