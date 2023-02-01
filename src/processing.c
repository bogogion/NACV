#include <apriltag/common/image_u8.h>
#include <apriltag/common/pnm.h>
#include <apriltag/common/math_util.h>
#include <stdio.h>
#include <stdlib.h>
#include "processing.h"


/* Generates real stride based off alignment */
int generate_stride(int width, unsigned int alignment)
{
	if((width % alignment) != 0)
	{
		width += alignment - (width % alignment);
	}
	return width;
}

/* Chaos. Remember kids, use ppm for safe rgb24 to rgb8/grey conversion! */
void convert_rgb24_proper(int width, int height, int stride, uint8_t *inbuf, image_u8_t *im)
{
	/* FUCKING SERIOUS IMPORTANT! Currently this function only works with RGB24, if you change this format, you'll need a different conversion
	 * (probably) */
	
	/* Conversion is (r + g + b)/3 */
	for(int y = 0; y < height; y++)
	{
		for(int x = 0; x < width; x++)
		{
			int r = inbuf[y*(width*3) + 3*x+0];
			int g = inbuf[y*(width*3) + 3*x+1];
			int b = inbuf[y*(width*3) + 3*x+2];

			im->buf[y*stride + x] = ((r + g + b)/3);
		}
	}
}

image_u8_t* create_image_u8(int width, int height, int stride)
{
	/* Gen buffer */
	uint8_t *buf = calloc(height*stride, sizeof(uint8_t));

	/* Constant magic */
	image_u8_t tmp = {.width = width, .height = height, .stride = stride, .buf = buf};

	image_u8_t *im = calloc(1, sizeof(image_u8_t));
	memcpy(im, &tmp, sizeof(image_u8_t));
	return im;
}
