#include <errno.h>
#include <stdlib.h>
#include "../lib/libv4l/include/libv4l2.h"

/* Apriltag detections! */
#include <apriltag/common/zarray.h>
#include <apriltag/apriltag.h>
#include <apriltag/common/image_u8.h>

/* Ids for camera settings. Potentially different for each camera. These values are for the uvcvideo (usb) drivers.*/
/* v4l2-ctl can be used to easily query the controls, or you could write code that does it for you ig. */

/* User Controls */
#define ID_BRIGHTNESS 		     0x00980900 
#define ID_CONTRAST	 	     0x00980901	
#define ID_SATURATION		     0x00980902
#define ID_SHARPNESS		     0x0098091b
#define ID_WHITE_BALANCE_AUTOMATIC   0x0098090c
#define ID_WHITE_BALANCE_TEMPERATURE 0x00980918  /* Requires white_balance_automatic to be 0 */

/* Camera controls, VIDIOC_S_EXT_CTRLS call */
/* Setting auto_exposure off is V4L2_EXPOSURE_MANUAL, on is V4L2_EXPOSURE_APERTURE_PRIORITY */
#define ID_AUTO_EXPOSURE	     0x009a0901 /* Type is of enum. List of enums is found in hell. Good luck! */
#define ID_EXPOSURE_TIME 	     0x009a0902

/* Add functionality for zoom and so on. Only included the useful ones*/

#define CLEAR(x) memset(&(x), 0, sizeof(x))

/* There will be multiple of these */
struct buffer {
	void      *start;
	size_t    length;
};

/* Ease of use function, used for safe v4l2_ calls */
void set_user_setting(uint32_t id, int32_t value);
void set_camera_setting(uint32_t id, int32_t value); /* EXT Call */
void xioctl(int fh, int request, void *arg);
zarray_t * get_detections(apriltag_detector_t *td,image_u8_t *im); /* Reminder; make sure to free the zarray_t generated*/
int init_everything(int width, int height, char *dev_name);
void init_mmap(); 
void set_cam_settings(int width, int height, int pformat); /* Used internally in init_cam */
int init_cam(char *dev_name, int width, int height); /* Return file descriptor */
void start_stream(int fd);
void close_cam(int fd); /* Cleans buffer too */
