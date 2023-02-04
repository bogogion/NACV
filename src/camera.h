#include <errno.h>
#include <stdlib.h>
#include "../lib/libv4l/include/libv4l2.h"

/* These variables */

#define CAMERA_WIDTH  640
#define CAMERA_HEIGHT 480

/* Apriltag detections! */
#include <apriltag/common/zarray.h>
#include <apriltag/apriltag.h>
#include <apriltag/common/image_u8.h>

/* User Control IDs. IDS are the same for each device if the device supports said setting. */

/* User Controls */
#define ID_BRIGHTNESS 		     0x00980900 
#define ID_CONTRAST	 	     0x00980901	
#define ID_SATURATION		     0x00980902
#define ID_SHARPNESS		     0x0098091b
#define ID_POWERLINE		     0x00980918 /* Frequency threshold for the camera, can help with screen tearing */

/* PI Controls */
#define ID_RED_BALANCE		     0x0098090e
#define ID_BLUE_BALANCE  	     0x0098090f
#define ID_COLOR_EFFECTS 	     0x0098091f /* 1-15, see v4l2-ctl --all --device /dev/video0 for list of values */
#define ID_COLOR_EFECTS_CBCR         0x0098092a


/* Camera controls, VIDIOC_S_EXT_CTRLS call */
#define ID_AUTO_EXPOSURE	     0x009a0901 /* 0: Auto Mode, 1: Manual Mode*/
#define ID_EXPOSURE_TIME 	     0x009a0902
#define ID_AUTO_EXPOSURE_BIAS        0x009a0903
#define ID_WHITE_BALANCE_AUTO_PRESET 0x009a0914
#define ID_IMAGE_STABILIZATION       0x009a0916
#define ID_ISO_SENSITIVITY           0x009a0917 /* Menu 0-4 */
#define ID_SCENE_MODE                0x009a091a /* Setting Presets */

#define ID_CLASS_USER		     0x980000
#define ID_CLASS_CAMERA		     0x9a0000

#define CLEAR(x) memset(&(x), 0, sizeof(x))

enum class_state{USER,CAMERA,DETECTOR};

struct buffer {
	void      *start;
	size_t    length;
};

/* Ease of use function, used for safe v4l2_ calls */
int set_settings_from_config(char *pathname); /* -1 for failure */
void set_camera_settings(uint32_t ctrl_class, uint32_t id, int32_t value); /* EXT Call */
void xioctl(int fh, int request, void *arg);
zarray_t * get_detections(apriltag_detector_t *td,image_u8_t *im); /* Reminder; make sure to free the zarray_t generated*/
int init_everything(int width, int height, char *dev_name);
void init_mmap(); 
void set_cam_settings(int width, int height, int pformat); /* Used internally in init_cam */
int init_cam(char *dev_name, int width, int height); /* Return file descriptor */
void start_stream(int fd);
void close_cam(int fd); /* Cleans buffer too */
