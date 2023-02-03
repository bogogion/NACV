CC = gcc
SRCS = main.* processing.* camera.*
LIB = -lapriltag -lm -lv4l2
TARGET = detector_camera
all: $(SRCS)
	$(CC) $(SRCS) $(LIB) -o $(TARGET)
install: $(TARGET)
	sudo cp $(TARGET) /usr/bin/
