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

<h2 align="center">Todo:</h2>

* [Hardware] Finish design for central control hub for interfacing 4 cameras to RoboRIO / PDH
* [Hardware] Build central control hub and publish schematics, documentation, parts, and pcb.
<<<<<<< HEAD
* [Camera]   Figure out how to interface the Pi Camera 3 with V4L2 (I suspect just update kernel on the Pi)
* [Camera]   Test all 3 cameras under one code.
=======
>>>>>>> 8fa2ff9 (quick update)
* [Camera]   Find the best settings for each camera with both low & high light conditions. Save it a setting file
* [Code]     Rewrite all camera code in libcamera C++ to support the newer cameras.
* [Code]     Finish writing calbibration code, the equations for it already are written.
* [Code]     Write script to install the auto-start service (systemd).
<<<<<<< HEAD
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
    * Takes in input from 4 pis. (Via Ethernet)
    * Packages this data into an ethernet frame to be sent back to RoboRIO.
    * Also provides power for all 4 Pis.
* Robo Rio
    * Takes data from CCH
    * Interprets and acts on that data in the Java code
=======
* [Code]     Figure out multiprocessing for faster apriltag processing
* [Math]     Figure out object detection from scratch if we have time. (maybe? apriltags are being used anyways)
* [Math]     Figure out pose for AprilTags.
>>>>>>> 8fa2ff9 (quick update)
