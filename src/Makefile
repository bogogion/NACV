CC = gcc
SRCS = main.*
C_SRCS = camera.*
LIB = -lapriltag -lm 
C_LIB = -lv4l2
TARGET = main
C_TARGET = camera
all: $(SRCS)
	$(CC) $(SRCS) $(LIB) -o $(TARGET)
camera: $(SRCS)
	$(CC) $(C_SRCS) $(C_LIB) -o camera
