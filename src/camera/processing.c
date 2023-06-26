#include <apriltag/common/image_u8.h>
#include <stdio.h>
#include <stdlib.h>
#include "processing.h"
#include "../camera/camera.h"
#include "../core/shared.h"

/* Generates real stride based off alignment */
int generate_stride(int width, unsigned int alignment)
{
	if((width % alignment) != 0)
	{
		width += alignment - (width % alignment);
	}
	return width;
}

void convert_rgb24_proper(int width, int height, int stride, uint8_t *inbuf, image_u8_t *im)
{
	/* Conversion, to my knowledge, is only applicable to 3-byte RGB format, such as RGB3 (rgb24)*/

	register int y;
	register int x;
	int r, g, b;


	/* Conversion is (r + g + b)/3 */
	for(y = 0; y < height; y++)
	{
		for(x = 0; x < width; x++)
		{
			r = inbuf[y*(width*3) + 3*x+0];
			g = inbuf[y*(width*3) + 3*x+1];
			b = inbuf[y*(width*3) + 3*x+2];

			/* Division trick, faster by a couple ms */
			im->buf[y*stride + x] = ((r + g + b) * 342 >> 10);
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

void destroy_image_u8(image_u8_t *im)
{
	free(im->buf);
	free(im);
}

void create_apriltag_stack(struct apriltag_stack *astack, int width, int height)
{
	astack->im = create_image_u8(width, height, generate_stride(width,DEFAULT_ALIGNMENT_U8));
	astack->tf = tag16h5_create();
	astack->td = apriltag_detector_create();

	/* Setup the detector */
	apriltag_detector_add_family(astack->td,astack->tf);
	astack->td->quad_decimate = 2.0;

}

void clean_apriltag_stack(struct apriltag_stack *astack)
{
	destroy_image_u8(astack->im);
	tag16h5_destroy(astack->tf);
	apriltag_detector_destroy(astack->td);
}

void launch_processes()
{
	char command[256];

	for(int i = 0; i < PROCESSORS; i++)
	{
		sprintf(command, "nacv-processor %i nacv_data %i nacv_ctrl &",i,SIZE);
		system(command);
		memset(command,0,256);
	}
}
