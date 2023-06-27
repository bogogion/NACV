<h1 align="center">NACV</h1>

<p align="center">Native AprilTag Computer Vision system written in C</p>

<p align="center"><b>Written by Zane Maples for the 3824 FRC Team (RoHawktics), can be used elsewhere</b></p>

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

* RaspberryPi 3 B/B+ - Officially Supported and tested on this platform
* RaspberryPi 2      - mileage may vary, even at high decoding and 4 processes only 20-25 FPS was processable
* RaspberryPi 4      - currently has not been tested. (note all Pi's must support V4L2 drivers or *legacy* camera stack)
* Pi Camera 1        - Officially Supported, however sensor mode must be manually set to 640x480 (Image with patched drivers will be released)
* 0V9281             - For best performance with AprilTags Monochrome + Global Shutter is recommended.

<h2 align="center">Needs To Be Tested</h2>
* RaspberryPi 4 - This is a low priority, I suspect that performance will be similar to Pi 3
* Orange Pi 5   - If ported correctly could provide insane performance

<h2 align="center">Todo:</h2>

* [Code] Write support for full options/configuration with cJSON [Misc]
* [Code] Write interfacing code / library for NACV and RoboRIO (either direct socket or NT3/4) [Comm]
* [Code] Rewrite AprilTag 3.2 to support GPU acceleration on Raspberry Pi [Performance]
* [Camera] Test with Pi Camera V1 until it works (may require patching of kernel) [Performance]
* [Camera] Wait and get the 0V9281 Monochrome sensor and fine tune program for that. [Performance]
* [Hardware] Develop/Print a case for Raspberry Pi 3 B/B+ [Custom]
* [Hardware] Develop an interface board for PDH to power 4 Pis. [Custom]
* [Hardware] (MAYBE) Finish CCU for the Pis (built in ethernet switch) [Potential]

<h2 align="center">System Diagram</h2>

* Code
    * Multi-processing
* Rasberry PI
    * Computer Vision code, written in C.
* Robo Rio
    * Takes data from PI
    * Interprets and acts on that data in the Java code
