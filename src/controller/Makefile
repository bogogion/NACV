CC = gcc
SRCS = ../core/shared.h ../camera/* ../math/* ../server/s_utils.* nacv-controller.c
LIB = -lapriltag -lm -lv4l2 -lpthread -lrt
IMAGE_DEBUG_TARGET=nacv-controller

all: $(SRCS)
	$(CC) $(SRCS) $(LIB) -O3 -o $(IMAGE_DEBUG_TARGET)
april_debug: 
	$(CC) $(SRCS) $(LIB) -O3 -o $(IMAGE_DEBUG_TARGET)
