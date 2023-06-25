<h1 align="center">NACV Multi-Processing</h1>


<h2 align="center">Technical Specifications</h2>

Each processor deals with a single AprilTag detector and one framebuffer. When the main process declares the buffers required each buffer is assigned
a single coprocessor. Currently there are support for just 4 coprocessors.

The main reasons for this system is as followed:
* Even with optimized algorithims for post-processing and AprilTag detection there still is a hard limit on how fast the detector can go. With
multiple coprocessors a higher stream of FPS can be handled and organized.
* It allows for insane speeds and detection. It can also be done with other vision pipelines such as object detection.

The main process handles a few things.
* It handles the thread for the server and communication
	* This thread handles WPILib NetworkTables and data formatting
* Coordinates the other processes with the camera's frames.

Data sharing is done with shm and frames are shared via memory mapping. Data structures are in shared.h
