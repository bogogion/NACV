CC = gcc
SRCS = main.* processing.* camera.*
LIB = -lapriltag -lm -lv4l2
TARGET = main
all: $(SRCS)
	$(CC) $(SRCS) $(LIB) -o $(TARGET)
