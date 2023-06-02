<h1 align="center">NACV</h1>

<p align="center">Native AprilTag Computer Vision system written in C</p>

<h2 align="center">Info</h2>

**NOTICE:**

This repo is currently being rewritten in order to support the 3 main PiCameras. The scope of this project will be shortened
to just being officially supported for Raspberry Pi's. 

This project as a whole will in the future include several pieces of hardware to support more than just one single camera. All of this math, 
code, and hardware will be posted within this repo.

Until further work is done consider most if not all of the documentation to be obsolete!

**Written for the FRC 3824 (HVA) team, can be used elsewhere**

*For specifics, and documentation:* [*Wiki*](https://github.com/bogogion/NACV/wiki)

<h2 align="center">Todo:</h2>

* [Hardware] Finish design for central control hub for interfacing 4 cameras to RoboRIO / PDH
* [Hardware] Build central control hub and publish schematics, documentation, parts, and pcb.
* [Camera]   Figure out how to interface the Pi Camera 3 with V4L2 (I suspect just update kernel on the Pi)
* [Camera]   Test all 3 cameras under one code.
* [Camera]   Find the best settings for each camera with both low & high light conditions. Save it a setting file
* [Code]     There were some issues with settings.
* [Code]     Finish writing calbibration code, the equations for it already are written.
* [Code]     Write script to install the auto-start service (systemd).
* [Math]     Figure out object detection from scratch if we have time. (maybe? apriltags are being used anyways)
* [Math]     Figure out pose for AprilTags.

<h2 align="center">System Diagram</h2>

* Code
    * /src/core = main file and test/debug files
    * /src/camera = camera code, processing code, and config for camera
    * /src/math = math for detections
    * /src/server = server code
    * /config/test.camcfg = test config for camera settings
* Rasberry PI
    * Computer Vision code, written in C.
* Central Control Hub (CCH)
    * Takes in input from 4 pis. (Via UART)
    * Packages this data into an ethernet frame to be sent back to RoboRIO.
    * Also provides power for all 4 Pis.
* Voltage Regulator Circuit
    * Takes in 12-15 V from battey along with a 15 AMP fuse on the PDH and converts/regulates it into 5Vs to be supplied to all of the devices.
* Robo Rio
    * Takes data from CCH
    * Interprets and acts on that data in the Java code
