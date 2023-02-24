# Native AprilTag Computer Vision system written in C

## For use by FRC team 3824 (HVA), but code is avaiable for any other teams

## MAJOR: This repo is currently a WIP parts of this readme may be inaccurate for the time being!

## Info
Camera input and processing handled by v4l2 (Video for Linux 2).

For all information (as I add it)

[Wiki](https://github.com/bogogion/frc-2023-cv/wiki)

## Current Todo:
- Write complete documentation on camera code for further maintence.
- Test.
- Figure out mounting on robot.
- Fix angle math function
- Add config for height/width/decision threshold

## Quick Use
`nacv` to run the program

`nacv -d` for debug images in working directory

By default the Pi will send packets from port 5805 (frc compatible)

## Troubleshooting
- `XIOCTL_CALL: Bad file descriptor` means your camera is not plugged in or it is under a different device name, run `ls /dev/ | grep "video"` to find
the correct device. (its often `video0`)
- Detector not detecting?
    - Make sure the tag is completely visible
    - Double check you are using the correct tag family (default is Tag16h5)
    - Make sure you are reading from the correct camera input (usually is not the problem, but if testing on a laptop can present problems)
    - Run the program `detector_camera -d` to print out debug pnms in the test directory
    - Change camera settings with the settings file
- Settings not properly being changed when using .camcfg
    - Double check it is the correct syntax and id
    - Try v4l2-ctl to set the control if all else fails
- No data coming through server
    - Make sure the client is reading from the correct ports
    - Ip addresses shouldn't matter too much but be sure both client and server and listening on localhost (127.0.0.1)

## System Diagram
* Code
    * /src/core = main file and test/debug files
    * /src/camera = camera code, processing code, and config for camera
    * /src/math = math for detections
    * /src/server = server code
    * /cfg/test.camcfg = test config for camera settings
* Rasberry PI
    * Computer Vision code, written in C.
    * Possible Image replaying (replication to other remote clients, ie. a controller)
* Robo Rio
    * Takes data from PI
    * Interprets and acts on that data in the Java code
