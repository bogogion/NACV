CC = gcc
SRCS = main.* processing.* camera.*
LIB = -lapriltag -lm -lv4l2
TARGET = main
FTARGET = camera
32 = -m32
all: $(SRCS)
	$(CC) $(SRCS) $(LIB) -o $(TARGET)
arm32: $(SRCS)
	$(CC) $(32) $(SRCS) $(LIB) -o $(FTARGET)
