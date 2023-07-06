<h1 align="center">NACV</h1>

<p align="center">Native AprilTag Computer Vision system written in C</p>

<p align="center"><b>Written by / for the 3824 FRC Team (RoHawktics), can be used elsewhere</b></p>

<h2 align="center">Info</h2>

**NOTICE:**

This project will be only officially supported on the Pi 3 B/B+ with the PiCamera 1. All development of drivers, code, and hardware
will be to support this board and camera. 

Eventually an official image will be released with patched kernel drivers to allow for higher framerate. A case is currently being produced
alongside a custom LED board to form a complete custom vision unit.

The software will also be built to support a custom version of AprilTag still in the works. Once all of these components are completed
all files will be public.

The project is being fundamentally rewritten to support complete multiprocessing of frames for accelerated peformance. This project will also be a 
*strictly* AprilTag vision system for the time being, however a headless (AprilTag less) version will be released to be used with OpenCV and other
vision pipelines.


<h2 align="center">Currently Supported Hardware</h2>

* RaspberryPi 3 B/B+ - Officially Supported and tested on this platform
* RaspberryPi 2      - mileage may vary, even at high decoding and 4 processes only 20-25 FPS was processable
* RaspberryPi 4      - currently has not been tested. (note all Pi's must support V4L2 drivers or *legacy* camera stack)
* Pi Camera 1        - Officially Supported, however sensor mode must be manually set to 640x480 (Image with patched drivers will be released)

<h2 align="center">Todo:</h2>

* [Code] Write support for full options/configuration with cJSON [Misc]
* [Code] Write Java/C++ library [RoboRIO]
* [Code] Re-write custom AprilTag implementation for better streaming [Performance]
* [Hardware] Develop/Print a case for Raspberry Pi 3 B/B+ [Custom]
* [Hardware] Develop an interface board for PDH to power 4 Pis. [Custom]
* [NACV] Post .stl files for case and kicad pcb project for ledboard [Misc]
* [Code] Write low-latency custom shared library to be loaded onto RoboRIO and into Java with native functions [Performance]

<h2 align="center">System Diagram</h2>

* Code
    * Multi-processing
* Rasberry PI
    * Computer Vision code, written in C.
* Robo Rio
    * Takes data from PI
    * Interprets and acts on that data in the Java code
