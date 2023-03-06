# NACV
> Native AprilTag Computer Vision system written in C

> *Written for the FRC 3824 (HVA) team, can be used elsewhere.*

## Info
*For specifics, and documentation:* [*Wiki*](https://github.com/bogogion/NACV/wiki)

## Installation guide
Download the latest release from [*AprilTag*](https://github.com/AprilRobotics/apriltag)
> Note, do not clone the repo, download the latest release and unzip it

Go into the *apriltag* directory and run

```shell
cmake -B build -DCMAKE_BUILD_TYPE=Release
sudo --build build --target install
```

Then go into `lib/libv4l/` inside the repo and run

```shell
make
sudo cp libv4l/libv4l2/*so* /usr/lib/
sudo cp libv4l/libv4lconvert/*so* /usr/lib/
```

To install NACV go into `src` and run 
```shell
make install
```

**Important**
> NACV sends all information over UDP to port 5805, see `exp/` to see a java example to put on RoboRIO

## Current Todo:
- Write complete documentation on camera code for further maintence.
- Test.
- Figure out mounting on robot.
- Add config for height/width/decision threshold

## Quick Use
`nacv` to run the program

`nacv -d` for debug images in working directory

To change memory type (userptr is fastest):

`nacv -u` for userptr or `nacv -m` for mmap (mmap is default, recommended to switch to -u if supported!)

By default the Pi will send packets from port 5805 to 5805 (frc compatible)
> Note, you will most likely have to go and change the destination address in `server/server_client.h under C_IP` to the address of the RoboRIO

## Troubleshooting
- `XIOCTL_CALL: Bad file descriptor` means your camera is not plugged in or it is under a different device name, run `ls /dev/ | grep "video"` to find
the correct device. (its often `video0`)
- `XIOCTL_CALL: Invalid Argument` on startup
    - Means that you are most likely using the wrong buffer type try `-m` or `-u` to change buffer type
- Detector not detecting?
    - Make sure the tag is completely visible
    - Double check you are using the correct tag family (default is Tag16h5)
    - Make sure you are reading from the correct camera input (usually is not the problem, but if testing on a laptop can present problems)
    - Run the program `nacv -d` to print out debug pnms in the test directory
    - Change camera settings with the settings file
- Settings not properly being changed when using .camcfg
    - Double check it is the correct syntax and id
    - Try v4l2-ctl to set the control if all else fails
- No data coming through server
    - Make sure the client is reading from the correct ports
    - Make sure that in `server/server_client.h` under `C_IP` the ip address is set to the RoboRIO
    - Make sure that the radio is properly configured, and that both devices are on the same subnet

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
