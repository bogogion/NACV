# Computer Vision module written in C for the HVA-FRC-3824 Robotics Team

# MAJOR: This repo is currently a WIP parts of this readme may be inaccurate for the time being!

## Info
Camera input and processing handled by v4l2 (Video for Linux 2).

camera.c & camera.h hold the code for the camera. (full documentation to be written)
The code essentially opens the camera, loads the device (/dev/video0 usually) into memory. This memory is made up of multiple mapped buffers that hold live data from the camera.

libv4l is also used [libv4l Github](https://github.com/philips/libv4l) for live processing. (Code is old and buggy, however still works,.)

main.c & main.h hold the main code.
This code is the main loop, it takes the life data from memory (generated from the camera code) and plugs it directly into AprilTag. It then handles the results and does math based on what is returned.
## Compilation
Makefile included in repository, updated as needed.

`make` to install, install option later added once code complete.

**IMPORTANT, the apriltag library is required to be built as a library and to have proper include paths.**

For instructions and github link: [Apriltag Github](https://github.com/AprilRobotics/apriltag)


## Current Todo:
- Remake Camera code to be cleaner and optimized.
- Write complete documentation on camera code for further maintence.
- Compile and import code onto Raspberry PI and mount on robot.
- Write code to communicate with the Java code (either analog pins or networking).
- Write conditions and math for detections.
- Test.

## System Diagram
* Rasberry PI
    * Computer Vision code, written in C.
    * Possible Image replaying (replication to other remote clients, ie. a controller)
* Robo Rio
    * Takes data from PI
    * Interprets and acts on that data in the Java code
