<h1 align="center">NACV</h1>

<p align="center">Native AprilTag Computer Vision system written in C</p>

## Info
> This was made for Raspberry Pi / Unix based systems.

**Written for the FRC 3824 (HVA) team, can be used elsewhere**

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
> NACV sends all information over UDP to port 5805, see `example/` to see a java example to put on RoboRIO

## Current Todo:
- **Write complete documentation on camera code for further maintence.**
- Doing => PUT CALIBRATION CONFIG FILE IN ANOTHER FILE!!
- Test.

## Quick Use

| Command | Desc |
| --- | --- |
| `nacv` | Default |
| `nacv -d` | Print out debug images |
| `nacv -m` | Run nacv with memory mapping (default) |
| `nacv -u` | Run nacv with userptr (fastest, but not as supporterd) |

By default the Pi will send packets from port 5805 to 5805 (frc compatible)
> Note, you will most likely have to go and change the destination address in `server/server_client.h under C_IP` to the address of the RoboRIO

## System Diagram
* Code
    * /src/core = main file and test/debug files
    * /src/camera = camera code, processing code, and config for camera
    * /src/math = math for detections
    * /src/server = server code
    * /config/test.camcfg = test config for camera settings
* Rasberry PI
    * Computer Vision code, written in C.
    * Possible Image replaying (replication to other remote clients, ie. a controller)
* Robo Rio
    * Takes data from PI
    * Interprets and acts on that data in the Java code
