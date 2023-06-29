#include <apriltag/common/image_u8.h>
#include <apriltag/common/math_util.h>
#include <apriltag/apriltag.h>
#include <apriltag/tag16h5.h>
#include <stdio.h>
#include <stdlib.h>

#ifndef PROCESSING_H
#define PROCESSING_H

struct apriltag_stack
{
	/* Holds all memory required for one detection */
	/* This helps eliminate mallocs and memcpys    */
	image_u8_t *im;
	

	apriltag_detector_t *td;
	apriltag_family_t *tf;
	apriltag_detection_t *det;
};

#define DEFAULT_ALIGNMENT_U8 96

int generate_stride(int width, unsigned int alignment);
void convert_rgb24_proper(int width, int height, int stride, uint8_t *inbuf, image_u8_t *im);
image_u8_t* create_image_u8(int width, int height, int stride);
void destroy_image_u8(image_u8_t *im);
void create_apriltag_stack(struct apriltag_stack *astack,int width, int height);
void clean_apriltag_stack(struct apriltag_stack *astack);
void launch_processes();

#endif
