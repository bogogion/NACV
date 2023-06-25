<h1 align="center">NACV</h1>

<p align="center">Native AprilTag Computer Vision system written in C</p>

<p align="center"><b>Written by Zane Maples for the 3824 FRC Team (RoHawktics), can be used elsewhere</b></p>

<p align="center"> ![NACV](https://img.shields.io/badge/NACV-green) </p>

<h2 align="center">Info</h2>

**NOTICE:**

This repo is undergoing a complete redo. The final code will most likely only support a small handfew of sensors, as the cameras must have support for 
V4L2 and support userpointer buffering.

The project is also being fundamentally rewritten to support complete multiprocessing of frames for accelerated peformance. This project will also be a 
*strictly* AprilTag vision system for the time being, however a headless (AprilTag less) version will be released to be used with OpenCV and other
vision pipelines.

This project as a whole will in the future include several pieces of hardware to support more than just one single camera. All of this math, 
code, and hardware will be posted within this repo.

<h2 align="center">Currently Supported Hardware</h2>

* RaspberryPi 3 B/B+
* RaspberryPi 2 will be tested
* RaspberryPi 4 currently has not been tested. (note all Pi's must support V4L2 drivers or *legacy* camera stack)
* Pi Camera 1 (currently has issues with mode setting)
* 0V9281 (any camera with the sensor) will be officially supported once testing begins


<h2 align="center">Todo:</h2>

* [Code] Write support for full options/configuration with cJSON
* [Code] Write interfacing code / library for NACV and RoboRIO (either direct socket or NT3/4)
* [Camera] Test with Pi Camera V1 until it works (may require patching of kernel)
* [Camera] Wait and get the 0V9281 Monochrome sensor and fine tune program for that.
* [Hardware] Develop/Print a case for Raspberry Pi 3 B/B+
* [Hardware] Develop an interface board for PDH to power 4 Pis.
* [Hardware] (MAYBE) Finish CCU for the Pis (built in ethernet switch)

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
