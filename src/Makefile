CC = gcc
SRCS = core/* camera/* math/* server/*
LIB = -lapriltag -lm -lv4l2
TARGET = nacv
all: $(SRCS)
	$(CC) $(SRCS) $(LIB) -o $(TARGET)
install: $(TARGET)
	sudo cp $(TARGET) /usr/bin/ && mkdir ~/.config/NACV && cp ../cfg/test.camcfg ~/.config/NACV/cam.camcfg
