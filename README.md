# Computer Vision module written in C for the HVA-FRC-3824 Robotics Team

## Compilation
Makefile included in repository, updated as needed.

`make` to install, install option later added once code complete.

**IMPORTANT, the apriltag library is required to be built as a library and to have proper include paths.**

For instructions and github link: [Apriltag Github](https://github.com/AprilRobotics/apriltag)

## Current Todo:
- Accurate AprilTag detection from long distances
- Image distance via calibration of image
- ~~Change Family to 16h11~~
- Create test enviroment for pictures
- Use video4linux API to natively process live video

## System Diagram
* Rasberry PI
    * Computer Vision code, written in C.
    * Possible Image replaying (replication to other remote clients, ie. a controller)
* Robo Rio
    * Robot Control
