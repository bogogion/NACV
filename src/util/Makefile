CC = gcc
SRCS = ../core/shared.h ../camera/* ../math/* ../server/* nacv-debugimage.c
LIB = -lapriltag -lm -lv4l2 -lpthread -lrt
IMAGE_DEBUG_TARGET=nacv-debugimage

all: $(SRCS)
	$(CC) $(SRCS) $(LIB) -O3 -o $(IMAGE_DEBUG_TARGET)
april_debug: 
	$(CC) $(SRCS) $(LIB) -O3 -o $(IMAGE_DEBUG_TARGET)
