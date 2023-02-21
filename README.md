# Computer Vision module written in C for the HVA-FRC-3824 Robotics Team

# MAJOR: This repo is currently a WIP parts of this readme may be inaccurate for the time being!

## Info
Camera input and processing handled by v4l2 (Video for Linux 2).

For all information (as I add it)

[Wiki](https://github.com/bogogion/frc-2023-cv/wiki)

## Current Todo:
- Write complete documentation on camera code for further maintence.
- Test.
- Figure out mounting on robot.

## System Diagram
* Code
    * /src/core = main file and test/debug files
    * /src/camera = camera code, processing code, and config for camera
    * /src/math = math for detections
    * /src/server = server code
* Rasberry PI
    * Computer Vision code, written in C.
    * Possible Image replaying (replication to other remote clients, ie. a controller)
* Robo Rio
    * Takes data from PI
    * Interprets and acts on that data in the Java code
