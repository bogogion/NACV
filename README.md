# Computer Vision module written in C for the HVA-FRC-3824 Robotics Team

# MAJOR: This repo is currently a WIP parts of this readme may be inaccurate for the time being!

## Info
Camera input and processing handled by v4l2 (Video for Linux 2).

For all information (as I add it)

[Wiki](https://github.com/bogogion/frc-2023-cv/wiki)

## Current Todo:
- Write complete documentation on camera code for further maintence.
- Write code to communicate with the Java code (either analog pins or networking).
- Write conditions and math for detections.
- Test.
- Figure out mounting on robot.
- Improve performance and tune detector for best detections.

## System Diagram
* Rasberry PI
    * Computer Vision code, written in C.
    * Possible Image replaying (replication to other remote clients, ie. a controller)
* Robo Rio
    * Takes data from PI
    * Interprets and acts on that data in the Java code
