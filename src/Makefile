CC = gcc
SRCS = main.* processing.* camera.*
C_SRCS = camera.*
LIB = -lapriltag -lm -lv4l2
C_LIB = -lv4l2
TARGET = main
C_TARGET = camera
all: $(SRCS)
	$(CC) $(SRCS) $(LIB) -o $(TARGET)
camera: $(SRCS)
	$(CC) $(C_SRCS) $(C_LIB) -o camera
